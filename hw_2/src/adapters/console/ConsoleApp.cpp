#include "ConsoleApp.hpp"

#include <algorithm>
#include <charconv>
#include <functional>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

#include "AssessmentFacade.hpp"

namespace
{
struct EndOfInput {};

class MenuCommand
{
public:
    MenuCommand(const char* label, std::function<void()> action)
        : label_(label), action_(std::move(action))
    {
    }

    const char* label() const
    {
        return label_;
    }

    void execute() const
    {
        action_();
    }

private:
    const char* label_;
    std::function<void()> action_;
};

template <typename Integer>
Integer parseInteger(std::string_view text)
{
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string_view::npos)
    {
        throw std::invalid_argument("Ожидается целое число");
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    text = text.substr(first, last - first + 1);

    Integer value{};
    const char* end = text.data() + text.size();
    const auto parsed = std::from_chars(text.data(), end, value);
    if (parsed.ec != std::errc{} || parsed.ptr != end)
    {
        throw std::invalid_argument("Ожидается целое число в допустимом диапазоне");
    }
    return value;
}

std::string formatPercentage(double percentage)
{
    std::ostringstream formatted;
    formatted.imbue(std::locale::classic());
    formatted << std::fixed << std::setprecision(2) << percentage << '%';
    return formatted.str();
}
}

ConsoleApp::ConsoleApp(AssessmentFacade& assessment, const ResultExporter& csv,
    const ResultExporter& tsv, std::istream& input, std::ostream& output)
    : assessment_(assessment), csv_(csv), tsv_(tsv), input_(input), output_(output)
{
}

void ConsoleApp::run()
{
    const std::vector<MenuCommand> commands{
        {"Добавить студента", [this] { addStudent(); }},
        {"Показать студентов", [this] { showStudents(); }},
        {"Добавить критерий", [this] { addCriterion(); }},
        {"Показать критерии", [this] { showCriteria(); }},
        {"Выставить оценку", [this] { saveEvaluation(false); }},
        {"Показать оценки студента", [this] { showEvaluations(); }},
        {"Исправить оценку", [this] { saveEvaluation(true); }},
        {"Отменить оценку", [this] { cancelEvaluation(); }},
        {"Показать итог студента", [this] { showResult(); }},
        {"Экспортировать итог в CSV", [this] { exportResult(csv_); }},
        {"Экспортировать итог в TSV", [this] { exportResult(tsv_); }}
    };

    output_ << "ПОС — журнал оценок\n";
    while (true)
    {
        output_ << '\n';
        for (std::size_t index = 0; index < commands.size(); ++index)
        {
            output_ << index + 1 << ". " << commands[index].label() << '\n';
        }
        output_ << "0. Выход\n";

        try
        {
            const auto choice = parseInteger<std::size_t>(readLine("Действие: "));
            if (choice == 0)
            {
                return;
            }
            if (choice > commands.size())
            {
                throw std::invalid_argument("Такого пункта меню нет");
            }
            commands[choice - 1].execute();
        }
        catch (const EndOfInput&)
        {
            return;
        }
        catch (const std::exception& error)
        {
            output_ << "Ошибка: " << error.what() << '\n';
        }
    }
}

std::string ConsoleApp::readLine(const char* prompt)
{
    output_ << prompt << std::flush;
    std::string line;
    if (!std::getline(input_, line))
    {
        throw EndOfInput{};
    }
    if (!line.empty() && line.back() == '\r')
    {
        line.pop_back();
    }
    return line;
}

double ConsoleApp::readWeight()
{
    auto text = readLine("Вес от 0 до 1: ");
    std::replace(text.begin(), text.end(), ',', '.');
    std::istringstream parser(text);
    parser.imbue(std::locale::classic());

    double weight = 0;
    if (!(parser >> weight))
    {
        throw std::invalid_argument("Ожидается числовой вес");
    }
    parser >> std::ws;
    if (!parser.eof())
    {
        throw std::invalid_argument("Ожидается числовой вес");
    }
    return weight;
}

std::uint64_t ConsoleApp::selectStudent()
{
    showStudents();
    return parseInteger<std::uint64_t>(readLine("ID студента: "));
}

std::uint64_t ConsoleApp::selectCriterion()
{
    showCriteria();
    return parseInteger<std::uint64_t>(readLine("ID критерия: "));
}

void ConsoleApp::addStudent()
{
    auto name = readLine("Имя: ");
    auto surname = readLine("Фамилия: ");
    const auto id = assessment_.addStudent(std::move(name), std::move(surname));
    output_ << "Студент добавлен. ID " << id << '\n';
}

void ConsoleApp::showStudents() const
{
    const auto students = assessment_.listStudents();
    if (students.empty())
    {
        output_ << "Список студентов пуст.\n";
    }
    for (const auto& student : students)
    {
        output_ << student.getId() << " — " << student.getName() << ' '
                << student.getSurname() << '\n';
    }
}

void ConsoleApp::addCriterion()
{
    auto name = readLine("Название критерия: ");
    const auto maxScore = parseInteger<int>(readLine("Максимальный балл: "));
    const auto weight = readWeight();
    const auto id = assessment_.addCriterion(std::move(name), maxScore, weight);
    output_ << "Критерий добавлен. ID " << id << '\n';
}

void ConsoleApp::showCriteria() const
{
    const auto criteria = assessment_.listCriteria();
    if (criteria.empty())
    {
        output_ << "Список критериев пуст.\n";
    }
    for (const auto& criterion : criteria)
    {
        output_ << criterion.getId() << " — " << criterion.getName()
                << " (максимум " << criterion.getMaxScore()
                << ", вес " << criterion.getWeight() << ")\n";
    }
}

void ConsoleApp::saveEvaluation(bool update)
{
    const auto studentId = selectStudent();
    const auto criterionId = selectCriterion();
    const auto score = parseInteger<int>(readLine("Балл: "));

    if (update)
    {
        assessment_.updateEvaluation(studentId, criterionId, score);
        output_ << "Оценка исправлена.\n";
    }
    else
    {
        assessment_.addEvaluation(studentId, criterionId, score);
        output_ << "Оценка выставлена.\n";
    }
}

void ConsoleApp::showEvaluations()
{
    const auto evaluations = assessment_.listEvaluations(selectStudent());
    if (evaluations.empty())
    {
        output_ << "Оценок пока нет.\n";
    }
    for (const auto& evaluation : evaluations)
    {
        const auto& criterion = assessment_.getCriterion(evaluation.getCriterionId());
        output_ << criterion.getId() << " — " << criterion.getName() << ": "
                << evaluation.getScore() << " из " << criterion.getMaxScore() << '\n';
    }
}

void ConsoleApp::cancelEvaluation()
{
    const auto studentId = selectStudent();
    const auto criterionId = selectCriterion();
    assessment_.cancelEvaluation(studentId, criterionId);
    output_ << "Оценка отменена.\n";
}

void ConsoleApp::showResult()
{
    const auto report = assessment_.result(selectStudent());
    const auto& grade = report.grade;
    output_ << report.student.getName() << ' ' << report.student.getSurname() << '\n';

    if (grade.status == FinalGradeStatus::ready)
    {
        output_ << "Итог: " << formatPercentage(grade.percentage) << '\n';
    }
    else if (grade.status == FinalGradeStatus::noWeightedCriteria)
    {
        output_ << "Итог не готов: нет критериев с положительным весом.\n";
    }
    else
    {
        output_ << "Итог не готов. Нет оценок по критериям:\n";
        for (const auto& criterion : grade.missingCriteria)
        {
            output_ << criterion.getId() << " — " << criterion.getName() << '\n';
        }
    }
}

void ConsoleApp::exportResult(const ResultExporter& exporter)
{
    const auto studentId = selectStudent();
    const auto path = readLine("Путь для экспорта: ");
    if (path.empty())
    {
        throw std::invalid_argument("Укажите путь для экспорта");
    }
    assessment_.exportResult(studentId, path, exporter);
    output_ << "Итог экспортирован.\n";
}
