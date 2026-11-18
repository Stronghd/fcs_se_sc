#pragma once

#include <map>

#include "EvaluationRepository.hpp"

class InMemoryEvaluationRepository final : public EvaluationRepository
{
public:
    const Evaluation* find(std::uint64_t studentId, std::uint64_t criterionId) const override;
    std::vector<Evaluation> list(std::uint64_t studentId) const override;
    void save(std::uint64_t studentId, const Evaluation& evaluation) override;
    void erase(std::uint64_t studentId, std::uint64_t criterionId) override;

private:
    std::map<std::uint64_t, std::vector<Evaluation>> evaluations_;
};
