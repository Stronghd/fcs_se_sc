#include "vyshkat/domain/ChargingCabinet.hpp"

#include <stdexcept>
#include <utility>

namespace vyshkat {

ChargingCabinet::ChargingCabinet(std::string inventoryNumber, std::string name, double energyKwh)
    : Item(std::move(inventoryNumber), std::move(name)), dailyEnergyKwh_(energyKwh) {
    if (dailyEnergyKwh_ < 0) {
        throw std::invalid_argument("Daily energy must be non-negative");
    }
}

double ChargingCabinet::dailyEnergyKwh() const { return dailyEnergyKwh_; }

} // namespace vyshkat