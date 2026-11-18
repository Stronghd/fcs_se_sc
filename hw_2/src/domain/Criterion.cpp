#include "Criterion.hpp"
#include <stdexcept>

// checks

bool Criterion::validateName(std::string name)
{
    return !name.empty();
}

bool Criterion::validateMaxScore(int maxScore)
{
    return maxScore > 0;
}

bool Criterion::validateWeight(double weight)
{
    return weight >= 0.0 && weight <= 1.0;
}

// конструктор

Criterion::Criterion(std::uint64_t id, std::string name, int maxScore, double weight) : id_(id), name_(name), maxScore_(maxScore), weight_(weight)
{
    if (!validateName(name_))
        throw std::invalid_argument("name must be not empty");

    if (!validateMaxScore(maxScore_))
        throw std::invalid_argument("maxScore must be above zero");

    if (!validateWeight(weight_))
        throw std::invalid_argument("weight must be between 0 - 1");
}

// гетеры

std::uint64_t Criterion::getId() const
{
    return id_;
}
std::string Criterion::getName() const
{
    return name_;
}
int Criterion::getMaxScore() const
{
    return maxScore_;
}
double Criterion::getWeight() const
{
    return weight_;
}
