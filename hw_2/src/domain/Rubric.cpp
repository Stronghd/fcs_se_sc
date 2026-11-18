#include "Rubric.hpp"

std::uint64_t Rubric::addCriterion(std::string name, int maxScore, double weight)
{
    const std::uint64_t id = nextId_;
    criteria_.emplace(id, Criterion(id, name, maxScore, weight));
    ++nextId_;
    return id;
}

const Criterion *Rubric::findCriterion(const std::uint64_t &id) const
{
    const auto answer = criteria_.find(id);
    if (answer != criteria_.end())
        return &(answer->second);
    else
        return nullptr;
}

std::vector<Criterion> Rubric::getCriteria() const
{
    std::vector<Criterion> answer;
    for (auto &criteria : criteria_)
    {
        answer.push_back(criteria.second);
    }
    return answer;
}