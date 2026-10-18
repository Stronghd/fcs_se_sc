
#include "vyshkat/domain/Vehicle.hpp"
#include <stdexcept>

namespace vyshkat {

Vehicle::Vehicle(std::string inventoryNumber, std::string name, int beginnerSimplicity)
    : Asset(std::move(inventoryNumber), std::move(name)), beginnerSimplicity_(beginnerSimplicity) {
    if (beginnerSimplicity > 10 || beginnerSimplicity < 1)
        throw std::invalid_argument("beginnerSimplicity out of range(1-10)");
}

int Vehicle::beginnerSimplicity() const { return beginnerSimplicity_; }

} // namespace vyshkat