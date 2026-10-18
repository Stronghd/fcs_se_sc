#pragma once

#include <string>

#include "vyshkat/domain/IEnergyConsumer.hpp"
#include "vyshkat/domain/Vehicle.hpp"

namespace vyshkat {

class Scooter : public Vehicle, public IEnergyConsumer {
public:
    Scooter(std::string inventoryNumber, std::string name, int beginnerSimplicity, double energyKwh);

    double dailyEnergyKwh() const override;

private:
    double dailyEnergyKwh_;
};
    
} // namespace vyshkat