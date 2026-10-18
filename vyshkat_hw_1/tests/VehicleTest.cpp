#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "vyshkat/domain/Vehicle.hpp"

namespace {

// Vehicle абстрактный, поэтому для теста нужен простой наследник
class TestVehicle : public vyshkat::Vehicle {
public:
    TestVehicle(std::string number, std::string name, int beginnerSimplicity)
        : Vehicle(std::move(number), std::move(name), beginnerSimplicity) {}
};

} // namespace

TEST(VehicleTest, StoresBeginnerSimplicity) {
    TestVehicle vehicle("INV-100", "Самокат", 7);
    EXPECT_EQ(vehicle.beginnerSimplicity(), 7);
    EXPECT_EQ(vehicle.inventoryNumber(), "INV-100");
    EXPECT_EQ(vehicle.name(), "Самокат");
}

TEST(VehicleTest, AcceptsBoundaryValues) {
    EXPECT_NO_THROW(TestVehicle("INV-101", "Самокат", 1));
    EXPECT_NO_THROW(TestVehicle("INV-102", "Самокат", 10));
}

TEST(VehicleTest, RejectsSimplicityBelowRange) {
    EXPECT_THROW(TestVehicle("INV-103", "Самокат", 0), std::invalid_argument);
}

TEST(VehicleTest, RejectsSimplicityAboveRange) {
    EXPECT_THROW(TestVehicle("INV-104", "Самокат", 11), std::invalid_argument);
}