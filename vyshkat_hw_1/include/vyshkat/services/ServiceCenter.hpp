#pragma once

#include "vyshkat/services/IServiceCenter.hpp"

namespace vyshkat {

class ServiceCenter : public IServiceCenter {
public:
    explicit ServiceCenter(double maxDailyEnergyKwh = 1.5, int minBeginnerSimplicity = 3);

    InspectionResult inspect(const Vehicle& vehicle) const override;

private:
    double maxDailyEnergyKwh_;
    int minBeginnerSimplicity_;
};

}  // namespace vyshkat
