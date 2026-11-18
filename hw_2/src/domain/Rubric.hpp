#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "Criterion.hpp"

class Rubric
{
public:
    std::uint64_t addCriterion(std::string name, int maxScore, double weight);
    const Criterion *findCriterion(const std::uint64_t &id) const;
    std::vector<Criterion> getCriteria() const;

private:
    std::map<std::uint64_t, Criterion> criteria_;
    std::uint64_t nextId_ = 1;
};