#pragma once

#include "vyshkat/domain/Vehicle.hpp"

namespace vyshkat {
class Bike : public Vehicle {
public:
    Bike(std::string inventoryNumber, std::string name, int beginnerSimplicity);
};
} // namespace vyshkat