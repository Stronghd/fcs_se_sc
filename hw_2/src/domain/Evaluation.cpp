#include "Evaluation.hpp"

#include "Criterion.hpp"

#include <stdexcept>

Evaluation::Evaluation(const Criterion& criterion, int score)
    : criterionId_(criterion.getId()), score_(score)
{
    if (score < 0 || score > criterion.getMaxScore())
    {
        throw std::invalid_argument("score must be between 0 and the criterion maximum");
    }
}

std::uint64_t Evaluation::getCriterionId() const
{
    return criterionId_;
}

int Evaluation::getScore() const
{
    return score_;
}
