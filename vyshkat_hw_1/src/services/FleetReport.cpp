#include "vyshkat/services/FleetReport.hpp"
#include "vyshkat/domain/IEnergyConsumer.hpp"
#include "vyshkat/domain/IInventoried.hpp"
#include "vyshkat/domain/Item.hpp"
#include "vyshkat/domain/Vehicle.hpp"
#include <cstddef>
#include <utility>
#include <vector>

namespace vyshkat {
FleetReport::FleetReport(std::shared_ptr<const Fleet> fleet, BeginnerPolicy policy)
    : fleet_(std::move(fleet)), policy_(std::move(policy)) {}


std::size_t FleetReport::totalCount() const { return fleet_->vehicles().size() + fleet_->items().size(); }

double FleetReport::totalDailyEnergy() const {
    double total = 0.0;
    for (const Vehicle *vehicle : fleet_->vehicles()) {
        const auto *consumer = dynamic_cast<const IEnergyConsumer *>(vehicle);
        if (consumer != nullptr)
            total += consumer->dailyEnergyKwh();
    }
    for (const Item *item : fleet_->items()) {
        const auto *consumer = dynamic_cast<const IEnergyConsumer *>(item);
        if (consumer != nullptr)
            total += consumer->dailyEnergyKwh();
    }
    return total;
}

std::vector<const Vehicle *> FleetReport::beginnerVehicles() const {
    std::vector<const Vehicle *> vehicles;
    for (const Vehicle *vehicle : fleet_->vehicles()) {
        if (policy_.isSuitable(*vehicle)) {
            vehicles.push_back(vehicle);
        }
    }
    return vehicles;
}

std::vector<const IInventoried *> FleetReport::inventoryList() const {
    std::vector<const IInventoried *> i_v;
    i_v.reserve(totalCount());
    for (const Vehicle *vehicle : fleet_->vehicles()) {
        i_v.push_back(vehicle);
    }
    for (const Item *item : fleet_->items()) {
        i_v.push_back(item);
    }
    return i_v;
}

} // namespace vyshkat