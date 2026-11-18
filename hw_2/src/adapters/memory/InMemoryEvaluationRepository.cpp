#include "InMemoryEvaluationRepository.hpp"

#include <algorithm>

namespace
{
template <typename Evaluations>
auto findByCriterion(Evaluations& evaluations, std::uint64_t criterionId)
{
    return std::find_if(evaluations.begin(), evaluations.end(),
        [criterionId](const Evaluation& evaluation) {
            return evaluation.getCriterionId() == criterionId;
        });
}
}

const Evaluation* InMemoryEvaluationRepository::find(
    std::uint64_t studentId, std::uint64_t criterionId) const
{
    const auto student = evaluations_.find(studentId);
    if (student == evaluations_.end())
    {
        return nullptr;
    }

    const auto& evaluations = student->second;
    const auto found = findByCriterion(evaluations, criterionId);
    return found == evaluations.end() ? nullptr : &*found;
}

std::vector<Evaluation> InMemoryEvaluationRepository::list(std::uint64_t studentId) const
{
    const auto student = evaluations_.find(studentId);
    if (student == evaluations_.end())
    {
        return {};
    }
    return student->second;
}

void InMemoryEvaluationRepository::save(std::uint64_t studentId, const Evaluation& evaluation)
{
    const auto student = evaluations_.find(studentId);
    if (student == evaluations_.end())
    {
        evaluations_.emplace(studentId, std::vector<Evaluation>{evaluation});
        return;
    }

    auto& evaluations = student->second;
    const auto found = findByCriterion(evaluations, evaluation.getCriterionId());
    if (found == evaluations.end())
    {
        evaluations.push_back(evaluation);
    }
    else
    {
        *found = evaluation;
    }
}

void InMemoryEvaluationRepository::erase(std::uint64_t studentId, std::uint64_t criterionId)
{
    const auto student = evaluations_.find(studentId);
    if (student == evaluations_.end())
    {
        return;
    }

    auto& evaluations = student->second;
    const auto found = findByCriterion(evaluations, criterionId);
    if (found != evaluations.end())
    {
        evaluations.erase(found);
        if (evaluations.empty())
        {
            evaluations_.erase(student);
        }
    }
}
