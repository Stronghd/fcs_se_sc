#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "ResultExporter.hpp"

class Gradebook;
class Rubric;
class StudentRepository;

class AssessmentFacade
{
public:
    AssessmentFacade(StudentRepository& students, Rubric& rubric, Gradebook& gradebook,
        const FinalGradeService& finalGrades);

    std::uint64_t addStudent(std::string name, std::string surname);
    std::vector<Student> listStudents() const;

    std::uint64_t addCriterion(std::string name, int maxScore, double weight);
    std::vector<Criterion> listCriteria() const;
    const Criterion& getCriterion(std::uint64_t criterionId) const;

    void addEvaluation(std::uint64_t studentId, std::uint64_t criterionId, int score);
    void updateEvaluation(std::uint64_t studentId, std::uint64_t criterionId, int score);
    void cancelEvaluation(std::uint64_t studentId, std::uint64_t criterionId);
    std::vector<Evaluation> listEvaluations(std::uint64_t studentId) const;

    ResultReport result(std::uint64_t studentId) const;
    void exportResult(std::uint64_t studentId, const std::string& path,
        const ResultExporter& exporter) const;

private:
    const Student& requireStudent(std::uint64_t studentId) const;

    StudentRepository& students_;
    Rubric& rubric_;
    Gradebook& gradebook_;
    const FinalGradeService& finalGrades_;
};
