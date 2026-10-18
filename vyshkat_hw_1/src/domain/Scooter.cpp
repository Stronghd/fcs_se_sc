#include "vyshkat/domain/Scooter.hpp"
#include <stdexcept>
#include <utility>

namespace vyshkat {
Scooter::Scooter(std::string inventoryNumber, std::string name, int beginnerSimplicity, double energyKwh)
    : Vehicle(std::move(inventoryNumber), std::move(name), beginnerSimplicity), dailyEnergyKwh_(energyKwh) {
    if (dailyEnergyKwh_ < 0)
        throw std::invalid_argument("EnergyKwh must be non-negative");
}
double Scooter::dailyEnergyKwh() const { return dailyEnergyKwh_; }
} // namespace vyshkat