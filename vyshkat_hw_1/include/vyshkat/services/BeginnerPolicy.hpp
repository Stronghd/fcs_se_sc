#pragma once

#include "vyshkat/domain/Vehicle.hpp"
namespace vyshkat {
class BeginnerPolicy {
public:
    explicit BeginnerPolicy(int minSimplicity = 6);
    bool isSuitable(const Vehicle &vehicle) const;

private:
    int minSimplicity_;
};
} // namespace vyshkat