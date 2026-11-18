#pragma once

#include <string>
#include <cstdint>

class Criterion
{
public:
    Criterion(std::uint64_t id, std::string name, int maxScore, double weight);
    std::uint64_t getId() const;
    std::string getName() const;
    int getMaxScore() const;
    double getWeight() const;

private:
    std::uint64_t id_;
    std::string name_;
    int maxScore_;
    double weight_;

    static bool validateName(std::string name);
    static bool validateMaxScore(int maxScore);
    static bool validateWeight(double weight);
};