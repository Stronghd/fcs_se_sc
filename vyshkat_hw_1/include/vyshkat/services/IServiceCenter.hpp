#pragma once

#include <string>

#include "vyshkat/domain/Vehicle.hpp"

namespace vyshkat {

struct InspectionResult {
    bool accepted;
    std::string reason;
};

class IServiceCenter {
public:
    virtual InspectionResult inspect(const Vehicle& vehicle) const = 0;
    virtual ~IServiceCenter() = default;
};

}  // namespace vyshkat
