#pragma once

#include "vyshkat/domain/IInventoried.hpp"
#include "vyshkat/services/BeginnerPolicy.hpp"
#include "vyshkat/services/Fleet.hpp"
#include <memory>
#include <vector>
#include <cstddef>

namespace vyshkat {

class FleetReport {
public:
    FleetReport(std::shared_ptr<const Fleet> fleet, BeginnerPolicy policy);
    std::size_t totalCount() const;
    double totalDailyEnergy() const;
    std::vector<const Vehicle *> beginnerVehicles() const;
    std::vector<const IInventoried *> inventoryList() const;

private:
    std::shared_ptr<const Fleet> fleet_;
    BeginnerPolicy policy_;
};
} // namespace vyshkat