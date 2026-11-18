#pragma once

#include <cstdint>
#include <vector>

#include "Evaluation.hpp"

class EvaluationRepository
{
public:
    virtual ~EvaluationRepository() = default;

    virtual const Evaluation* find(std::uint64_t studentId, std::uint64_t criterionId) const = 0;
    virtual std::vector<Evaluation> list(std::uint64_t studentId) const = 0;
    virtual void save(std::uint64_t studentId, const Evaluation& evaluation) = 0;
    virtual void erase(std::uint64_t studentId, std::uint64_t criterionId) = 0;
};
