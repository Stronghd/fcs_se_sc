#pragma once

#include "vyshkat/domain/Item.hpp"
#include "vyshkat/domain/Vehicle.hpp"
#include "vyshkat/services/Fleet.hpp"
#include "vyshkat/services/IServiceCenter.hpp"
#include <memory>
namespace vyshkat {

class FleetService {
public:
    FleetService(std::shared_ptr<Fleet> fleet, std::shared_ptr<const IServiceCenter> serviceCenter);
    InspectionResult acceptVehicle(std::unique_ptr<Vehicle> vehicle);
    InspectionResult addItem(std::unique_ptr<Item> item);

private:
    std::shared_ptr<Fleet> fleet_;
    std::shared_ptr<const IServiceCenter> serviceCenter_;
};

} // namespace vyshkat