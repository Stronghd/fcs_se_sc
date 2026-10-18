#include "vyshkat/services/ServiceCenter.hpp"

#include <string>

#include "vyshkat/domain/IEnergyConsumer.hpp"

namespace vyshkat {

ServiceCenter::ServiceCenter(double maxDailyEnergyKwh, int minBeginnerSimplicity)
    : maxDailyEnergyKwh_(maxDailyEnergyKwh), minBeginnerSimplicity_(minBeginnerSimplicity) {}

InspectionResult ServiceCenter::inspect(const Vehicle &vehicle) const {
    if (const auto *consumer = dynamic_cast<const IEnergyConsumer *>(&vehicle)) {
        if (consumer->dailyEnergyKwh() > maxDailyEnergyKwh_) {
            return {false, "Расход " + std::to_string(consumer->dailyEnergyKwh()) +
                               " кВт*ч/сутки выше допустимого"};
        }
    }
    if (vehicle.beginnerSimplicity() < minBeginnerSimplicity_) {
        return {false, "Слишком сложное управление для проката (простота " +
                           std::to_string(vehicle.beginnerSimplicity()) + ")"};
    }
    return {true, ""};
}

} // namespace vyshkat
