#include "vyshkat/domain/EBike.hpp"
#include <stdexcept>
#include <utility>

namespace vyshkat {
EBike::EBike(std::string inventoryNumber, std::string name, int beginnerSimplicity, double energyKwh)
    : Vehicle(std::move(inventoryNumber), std::move(name), beginnerSimplicity), dailyEnergyKwh_(energyKwh) {
    if (dailyEnergyKwh_ < 0)
        throw std::invalid_argument("EnergyKwh must be non-negative");
}
double EBike::dailyEnergyKwh() const { return dailyEnergyKwh_; }
} // namespace vyshkat