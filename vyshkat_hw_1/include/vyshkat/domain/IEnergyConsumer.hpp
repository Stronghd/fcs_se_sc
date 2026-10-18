#pragma once

namespace vyshkat {

class IEnergyConsumer {
public:
    virtual double dailyEnergyKwh() const = 0;
    virtual ~IEnergyConsumer() = default;
};
} // namespace vyshkat