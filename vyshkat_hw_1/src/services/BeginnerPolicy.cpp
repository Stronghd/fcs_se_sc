#include "vyshkat/services/BeginnerPolicy.hpp"
namespace vyshkat {

BeginnerPolicy::BeginnerPolicy(int minSimplicity) : minSimplicity_(minSimplicity) {}

bool BeginnerPolicy::isSuitable(const Vehicle &vehicle) const { return vehicle.beginnerSimplicity() >= minSimplicity_; }
} // namespace vyshkat  