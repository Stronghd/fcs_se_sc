#include "Gradebook.hpp"

#include <stdexcept>

#include "EvaluationRepository.hpp"
#include "Rubric.hpp"
#include "StudentRepository.hpp"

Gradebook::Gradebook(const StudentRepository& students, const Rubric& rubric,
    EvaluationRepository& evaluations)
    : students_(students), rubric_(rubric), evaluations_(evaluations)
{
}

void Gradebook::addEvaluation(std::uint64_t studentId, std::uint64_t criterionId, int score)
{
    if (students_.findStudent(studentId) == nullptr)
    {
        throw std::invalid_argument("student ID not found");
    }

    const Evaluation evaluation(requireCriterion(criterionId), score);
    if (evaluations_.find(studentId, criterionId) != nullptr)
    {
        throw std::invalid_argument("student already has an evaluation for this criterion");
    }

    evaluations_.save(studentId, evaluation);
}

void Gradebook::updateEvaluation(std::uint64_t studentId, std::uint64_t criterionId, int score)
{
    requireEvaluation(studentId, criterionId);
    evaluations_.save(studentId, Evaluation(requireCriterion(criterionId), score));
}

void Gradebook::cancelEvaluation(std::uint64_t studentId, std::uint64_t criterionId)
{
    requireEvaluation(studentId, criterionId);
    evaluations_.erase(studentId, criterionId);
}

std::vector<Evaluation> Gradebook::getEvaluations(std::uint64_t studentId) const
{
    return evaluations_.list(studentId);
}

const Criterion& Gradebook::requireCriterion(std::uint64_t criterionId) const
{
    const Criterion* criterion = rubric_.findCriterion(criterionId);
    if (criterion == nullptr)
    {
        throw std::invalid_argument("criterion ID not found");
    }

    return *criterion;
}

void Gradebook::requireEvaluation(std::uint64_t studentId, std::uint64_t criterionId) const
{
    if (evaluations_.find(studentId, criterionId) == nullptr)
    {
        throw std::invalid_argument("evaluation not found");
    }
}
