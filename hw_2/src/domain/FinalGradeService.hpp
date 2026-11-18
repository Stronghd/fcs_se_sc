#pragma once

#include <vector>

#include "Criterion.hpp"
#include "Evaluation.hpp"

enum class FinalGradeStatus
{
    ready,
    missingEvaluations,
    noWeightedCriteria
};

struct FinalGrade
{
    FinalGradeStatus status;
    double percentage = 0;
    std::vector<Criterion> missingCriteria;
};

class FinalGradeService
{
public:
    FinalGrade calculate(const std::vector<Criterion>& criteria,
        const std::vector<Evaluation>& evaluations) const;
};
