#include "AssessmentFacade.hpp"

#include <stdexcept>
#include <utility>

#include "Gradebook.hpp"
#include "Rubric.hpp"
#include "StudentRepository.hpp"

AssessmentFacade::AssessmentFacade(StudentRepository& students, Rubric& rubric,
    Gradebook& gradebook, const FinalGradeService& finalGrades)
    : students_(students), rubric_(rubric), gradebook_(gradebook), finalGrades_(finalGrades)
{
}

std::uint64_t AssessmentFacade::addStudent(std::string name, std::string surname)
{
    return students_.addStudent(std::move(name), std::move(surname));
}

std::vector<Student> AssessmentFacade::listStudents() const
{
    return students_.getStudents();
}

std::uint64_t AssessmentFacade::addCriterion(std::string name, int maxScore, double weight)
{
    return rubric_.addCriterion(std::move(name), maxScore, weight);
}

std::vector<Criterion> AssessmentFacade::listCriteria() const
{
    return rubric_.getCriteria();
}

const Criterion& AssessmentFacade::getCriterion(std::uint64_t criterionId) const
{
    const auto* criterion = rubric_.findCriterion(criterionId);
    if (criterion == nullptr)
    {
        throw std::invalid_argument("Критерий с таким ID не найден");
    }
    return *criterion;
}

void AssessmentFacade::addEvaluation(std::uint64_t studentId, std::uint64_t criterionId, int score)
{
    gradebook_.addEvaluation(studentId, criterionId, score);
}

void AssessmentFacade::updateEvaluation(std::uint64_t studentId, std::uint64_t criterionId, int score)
{
    gradebook_.updateEvaluation(studentId, criterionId, score);
}

void AssessmentFacade::cancelEvaluation(std::uint64_t studentId, std::uint64_t criterionId)
{
    gradebook_.cancelEvaluation(studentId, criterionId);
}

std::vector<Evaluation> AssessmentFacade::listEvaluations(std::uint64_t studentId) const
{
    requireStudent(studentId);
    return gradebook_.getEvaluations(studentId);
}

ResultReport AssessmentFacade::result(std::uint64_t studentId) const
{
    const Student& student = requireStudent(studentId);
    return {student, finalGrades_.calculate(rubric_.getCriteria(),
        gradebook_.getEvaluations(studentId))};
}

void AssessmentFacade::exportResult(std::uint64_t studentId, const std::string& path,
    const ResultExporter& exporter) const
{
    const auto report = result(studentId);
    if (report.grade.status != FinalGradeStatus::ready)
    {
        throw std::invalid_argument("Итог не готов к экспорту");
    }
    exporter.write(path, report);
}

const Student& AssessmentFacade::requireStudent(std::uint64_t studentId) const
{
    const auto* student = students_.findStudent(studentId);
    if (student == nullptr)
    {
        throw std::invalid_argument("Студент с таким ID не найден");
    }
    return *student;
}
