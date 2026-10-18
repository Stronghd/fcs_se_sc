#include <gtest/gtest.h>

#include <memory>

#include "vyshkat/domain/Bike.hpp"
#include "vyshkat/domain/DockStation.hpp"
#include "vyshkat/domain/Helmet.hpp"
#include "vyshkat/domain/Scooter.hpp"
#include "vyshkat/services/Fleet.hpp"

using vyshkat::Bike;
using vyshkat::DockStation;
using vyshkat::Fleet;
using vyshkat::Helmet;
using vyshkat::Scooter;

TEST(FleetTest, StartsEmpty) {
    Fleet fleet;
    EXPECT_TRUE(fleet.vehicles().empty());
    EXPECT_TRUE(fleet.items().empty());
}

TEST(FleetTest, AddsVehicle) {
    Fleet fleet;
    EXPECT_TRUE(fleet.addVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4)));
    ASSERT_EQ(fleet.vehicles().size(), 1u);
    EXPECT_EQ(fleet.vehicles()[0]->inventoryNumber(), "SC-001");
    EXPECT_TRUE(fleet.contains("SC-001"));
}

TEST(FleetTest, AddsItem) {
    Fleet fleet;
    EXPECT_TRUE(fleet.addItem(std::make_unique<Helmet>("HL-001", "Шлем")));
    ASSERT_EQ(fleet.items().size(), 1u);
    EXPECT_EQ(fleet.items()[0]->name(), "Шлем");
}

TEST(FleetTest, RejectsDuplicateVehicleNumber) {
    Fleet fleet;
    fleet.addVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4));
    EXPECT_FALSE(fleet.addVehicle(std::make_unique<Bike>("SC-001", "Велосипед", 8)));
    EXPECT_EQ(fleet.vehicles().size(), 1u);
}


TEST(FleetTest, RejectsItemWithVehicleNumber) {
    Fleet fleet;
    fleet.addVehicle(std::make_unique<Scooter>("INV-001", "Самокат", 7, 0.4));
    EXPECT_FALSE(fleet.addItem(std::make_unique<Helmet>("INV-001", "Шлем")));
    EXPECT_TRUE(fleet.items().empty());
}

TEST(FleetTest, RejectsVehicleWithItemNumber) {
    Fleet fleet;
    fleet.addItem(std::make_unique<DockStation>("INV-002", "Док-станция", 0.8));
    EXPECT_FALSE(fleet.addVehicle(std::make_unique<Bike>("INV-002", "Велосипед", 8)));
    EXPECT_TRUE(fleet.vehicles().empty());
}

TEST(FleetTest, StoresVehiclesAndItemsSeparately) {
    Fleet fleet;
    fleet.addVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4));
    fleet.addVehicle(std::make_unique<Bike>("BK-001", "Велосипед", 8));
    fleet.addItem(std::make_unique<Helmet>("HL-001", "Шлем"));
    EXPECT_EQ(fleet.vehicles().size(), 2u);
    EXPECT_EQ(fleet.items().size(), 1u);
}

TEST(FleetTest, ContainsReturnsFalseForUnknownNumber) {
    Fleet fleet;
    fleet.addVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4));
    EXPECT_FALSE(fleet.contains("NOPE-404"));
}
