#include <gtest/gtest.h>

#include "vyshkat/domain/Bike.hpp"
#include "vyshkat/domain/EBike.hpp"
#include "vyshkat/domain/Scooter.hpp"
#include "vyshkat/services/ServiceCenter.hpp"

using vyshkat::Bike;
using vyshkat::EBike;
using vyshkat::Scooter;
using vyshkat::ServiceCenter;

TEST(ServiceCenterTest, AcceptsNormalScooter) {
    ServiceCenter center;
    Scooter scooter("SC-001", "Самокат", 7, 0.4);
    const auto result = center.inspect(scooter);
    EXPECT_TRUE(result.accepted);
}

TEST(ServiceCenterTest, RejectsScooterWithHighEnergy) {
    ServiceCenter center;
    Scooter scooter("SC-002", "Самокат", 7, 2.0);
    const auto result = center.inspect(scooter);
    EXPECT_FALSE(result.accepted);
    EXPECT_FALSE(result.reason.empty());
}

TEST(ServiceCenterTest, AcceptsEnergyExactlyAtLimit) {
    ServiceCenter center;
    EBike ebike("EB-001", "Электровелосипед", 6, 1.5);
    EXPECT_TRUE(center.inspect(ebike).accepted);
}

// Велосипед не потребляет энергию: правило расхода к нему не применяется
TEST(ServiceCenterTest, AcceptsBikeWithoutEnergyCheck) {
    ServiceCenter center;
    Bike bike("BK-001", "Велосипед", 8);
    EXPECT_TRUE(center.inspect(bike).accepted);
}

TEST(ServiceCenterTest, RejectsTooComplexVehicle) {
    ServiceCenter center;
    Bike bike("BK-002", "Шоссейный велосипед", 2);
    const auto result = center.inspect(bike);
    EXPECT_FALSE(result.accepted);
    EXPECT_FALSE(result.reason.empty());
}

TEST(ServiceCenterTest, UsesLimitsFromConstructor) {
    ServiceCenter strictCenter(0.3, 5);
    Scooter scooter("SC-003", "Самокат", 7, 0.4);
    EXPECT_FALSE(strictCenter.inspect(scooter).accepted);
}
