#include "FinalGradeService.hpp"

#include <cstdint>
#include <unordered_map>

FinalGrade FinalGradeService::calculate(const std::vector<Criterion>& criteria,
    const std::vector<Evaluation>& evaluations) const
{
    std::unordered_map<std::uint64_t, int> scores;
    for (const auto& evaluation : evaluations)
    {
        scores.emplace(evaluation.getCriterionId(), evaluation.getScore());
    }

    FinalGrade result{FinalGradeStatus::noWeightedCriteria, 0, {}};
    long double totalWeight = 0;
    long double weightedScore = 0;

    for (const auto& criterion : criteria)
    {
        const long double weight = criterion.getWeight();
        if (weight == 0)
        {
            continue;
        }

        totalWeight += weight;
        const auto score = scores.find(criterion.getId());
        if (score == scores.end())
        {
            result.missingCriteria.push_back(criterion);
            continue;
        }

        const auto fraction = static_cast<long double>(score->second) / criterion.getMaxScore();
        weightedScore += weight * fraction;
    }

    if (!result.missingCriteria.empty())
    {
        result.status = FinalGradeStatus::missingEvaluations;
    }
    else if (totalWeight > 0)
    {
        result.status = FinalGradeStatus::ready;
        result.percentage = static_cast<double>(100 * (weightedScore / totalWeight));
    }

    return result;
}
