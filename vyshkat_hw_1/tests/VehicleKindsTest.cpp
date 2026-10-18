#include <gtest/gtest.h>

#include <stdexcept>

#include "vyshkat/domain/Bike.hpp"
#include "vyshkat/domain/EBike.hpp"
#include "vyshkat/domain/IEnergyConsumer.hpp"
#include "vyshkat/domain/Scooter.hpp"
#include "vyshkat/domain/Vehicle.hpp"

using vyshkat::Bike;
using vyshkat::EBike;
using vyshkat::IEnergyConsumer;
using vyshkat::Scooter;
using vyshkat::Vehicle;

TEST(ScooterTest, StoresDailyEnergy) {
    Scooter scooter("SC-001", "Самокат", 7, 0.4);
    EXPECT_DOUBLE_EQ(scooter.dailyEnergyKwh(), 0.4);
    EXPECT_EQ(scooter.beginnerSimplicity(), 7);
}

TEST(ScooterTest, AcceptsZeroEnergy) { EXPECT_NO_THROW(Scooter("SC-002", "Самокат", 7, 0.0)); }

TEST(ScooterTest, RejectsNegativeEnergy) { EXPECT_THROW(Scooter("SC-003", "Самокат", 7, -0.1), std::invalid_argument); }

TEST(EBikeTest, StoresDailyEnergy) {
    EBike ebike("EB-001", "Электровелосипед", 5, 1.2);
    EXPECT_DOUBLE_EQ(ebike.dailyEnergyKwh(), 1.2);
}

TEST(EBikeTest, RejectsNegativeEnergy) {
    EXPECT_THROW(EBike("EB-002", "Электровелосипед", 5, -1.0), std::invalid_argument);
}

TEST(BikeTest, StoresCommonFields) {
    Bike bike("BK-001", "Велосипед", 8);
    EXPECT_EQ(bike.inventoryNumber(), "BK-001");
    EXPECT_EQ(bike.name(), "Велосипед");
    EXPECT_EQ(bike.beginnerSimplicity(), 8);
}

TEST(VehicleKindsTest, BikeIsNotEnergyConsumer) {
    Bike bike("BK-002", "Велосипед", 8);
    const Vehicle &vehicle = bike;
    EXPECT_EQ(dynamic_cast<const IEnergyConsumer *>(&vehicle), nullptr);
}

TEST(VehicleKindsTest, ScooterAndEBikeAreEnergyConsumers) {
    Scooter scooter("SC-004", "Самокат", 7, 0.4);
    EBike ebike("EB-003", "Электровелосипед", 5, 1.2);
    const Vehicle &scooterAsVehicle = scooter;
    const Vehicle &ebikeAsVehicle = ebike;
    EXPECT_NE(dynamic_cast<const IEnergyConsumer *>(&scooterAsVehicle), nullptr);
    EXPECT_NE(dynamic_cast<const IEnergyConsumer *>(&ebikeAsVehicle), nullptr);
}
