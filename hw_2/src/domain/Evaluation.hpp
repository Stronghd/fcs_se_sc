#pragma once

#include <cstdint>

class Criterion;

class Evaluation
{
public:
    Evaluation(const Criterion& criterion, int score);

    std::uint64_t getCriterionId() const;
    int getScore() const;

private:
    std::uint64_t criterionId_;
    int score_;
};
