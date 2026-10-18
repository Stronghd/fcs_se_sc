#pragma once

#include <gmock/gmock.h>

#include "vyshkat/domain/Vehicle.hpp"
#include "vyshkat/services/IServiceCenter.hpp"

namespace vyshkat {

class MockServiceCenter : public IServiceCenter {
public:
    MOCK_METHOD(InspectionResult, inspect, (const Vehicle &vehicle), (const, override));
};

} // namespace vyshkat
