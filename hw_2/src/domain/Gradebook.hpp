#pragma once

#include <cstdint>
#include <vector>

#include "Evaluation.hpp"

class Criterion;
class EvaluationRepository;
class Rubric;
class StudentRepository;

class Gradebook
{
public:
    Gradebook(const StudentRepository& students, const Rubric& rubric,
        EvaluationRepository& evaluations);

    void addEvaluation(std::uint64_t studentId, std::uint64_t criterionId, int score);
    void updateEvaluation(std::uint64_t studentId, std::uint64_t criterionId, int score);
    void cancelEvaluation(std::uint64_t studentId, std::uint64_t criterionId);
    std::vector<Evaluation> getEvaluations(std::uint64_t studentId) const;

private:
    const Criterion &requireCriterion(std::uint64_t criterionId) const;
    void requireEvaluation(std::uint64_t studentId, std::uint64_t criterionId) const;

    const StudentRepository& students_;
    const Rubric &rubric_;
    EvaluationRepository& evaluations_;
};
