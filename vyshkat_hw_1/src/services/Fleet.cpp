#include "vyshkat/services/Fleet.hpp"
#include "vyshkat/domain/Vehicle.hpp"
#include <memory>
namespace vyshkat {
bool Fleet::contains(const std::string &inventoryNumber) const {
    for (const auto &v : vehicles_) {
        if (v->inventoryNumber() == inventoryNumber)
            return true;
    }
    for (const auto &v : items_) {
        if (v->inventoryNumber() == inventoryNumber)
            return true;
    }
    return false;
}

bool Fleet::addVehicle(std::unique_ptr<Vehicle> vehicle) {
    if (contains(vehicle->inventoryNumber()))
        return false;
    vehicles_.push_back(std::move(vehicle));
    return true;
}

bool Fleet::addItem(std::unique_ptr<Item> item) {
    if (contains(item->inventoryNumber()))
        return false;
    items_.push_back(std::move(item));
    return true;
}

std::vector<const Vehicle *> Fleet::vehicles() const {
    std::vector<const Vehicle *> result;
    result.reserve(vehicles_.size());
    for (const auto &v : vehicles_)
        result.push_back(v.get());
    return result;
}


std::vector<const Item *> Fleet::items() const {
    std::vector<const Item *> result;
    result.reserve(items_.size());
    for (const auto &v : items_)
        result.push_back(v.get());
    return result;
}


} // namespace vyshkat