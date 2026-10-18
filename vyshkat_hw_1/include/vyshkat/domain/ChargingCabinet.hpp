#pragma once

#include <string>

#include "vyshkat/domain/IEnergyConsumer.hpp"
#include "vyshkat/domain/Item.hpp"

namespace vyshkat {

class ChargingCabinet : public Item, public IEnergyConsumer {
public:
    ChargingCabinet(std::string inventoryNumber, std::string name, double energyKwh);

    double dailyEnergyKwh() const override;

private:
    double dailyEnergyKwh_;
};

} // namespace vyshkat