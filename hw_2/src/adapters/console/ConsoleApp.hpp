#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

class AssessmentFacade;
class ResultExporter;

class ConsoleApp
{
public:
    ConsoleApp(AssessmentFacade& assessment, const ResultExporter& csv,
        const ResultExporter& tsv, std::istream& input, std::ostream& output);

    void run();

private:
    std::string readLine(const char* prompt);
    double readWeight();
    std::uint64_t selectStudent();
    std::uint64_t selectCriterion();

    void addStudent();
    void showStudents() const;
    void addCriterion();
    void showCriteria() const;
    void saveEvaluation(bool update);
    void showEvaluations();
    void cancelEvaluation();
    void showResult();
    void exportResult(const ResultExporter& exporter);

    AssessmentFacade& assessment_;
    const ResultExporter& csv_;
    const ResultExporter& tsv_;
    std::istream& input_;
    std::ostream& output_;
};
