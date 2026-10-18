#include "vyshkat/services/FleetService.hpp"
#include "vyshkat/domain/Item.hpp"
#include "vyshkat/services/IServiceCenter.hpp"
#include <memory>
#include <utility>
namespace vyshkat {
FleetService::FleetService(std::shared_ptr<Fleet> fleet, std::shared_ptr<const IServiceCenter> serviceCenter)
    : fleet_(std::move(fleet)), serviceCenter_(std::move(serviceCenter)) {}

InspectionResult FleetService::acceptVehicle(std::unique_ptr<Vehicle> vehicle) {
    InspectionResult result = serviceCenter_->inspect(*vehicle);
    if (!result.accepted)
        return result;
    bool f = fleet_->addVehicle(std::move(vehicle));
    if (!f)
        return {false, "дубликат"};
    return {true, ""};
}

InspectionResult FleetService::addItem(std::unique_ptr<Item> item) {
    bool f = fleet_->addItem(std::move(item));
    if (!f)
        return {false, "дубликат"};
    return {true, ""};
}

} // namespace vyshkat