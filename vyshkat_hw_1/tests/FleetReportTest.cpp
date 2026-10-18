#include <gtest/gtest.h>

#include <memory>

#include "vyshkat/domain/Bike.hpp"
#include "vyshkat/domain/DockStation.hpp"
#include "vyshkat/domain/Helmet.hpp"
#include "vyshkat/domain/Scooter.hpp"
#include "vyshkat/services/BeginnerPolicy.hpp"
#include "vyshkat/services/Fleet.hpp"
#include "vyshkat/services/FleetReport.hpp"

using vyshkat::BeginnerPolicy;
using vyshkat::Bike;
using vyshkat::DockStation;
using vyshkat::Fleet;
using vyshkat::FleetReport;
using vyshkat::Helmet;
using vyshkat::Scooter;

TEST(FleetReportTest, EmptyFleetHasZeroCountAndEnergy) {
    auto fleet = std::make_shared<Fleet>();
    FleetReport report(fleet, BeginnerPolicy());

    EXPECT_EQ(report.totalCount(), 0u);
    EXPECT_DOUBLE_EQ(report.totalDailyEnergy(), 0.0);
    EXPECT_TRUE(report.inventoryList().empty());
}

TEST(FleetReportTest, CountsVehiclesAndItemsTogether) {
    auto fleet = std::make_shared<Fleet>();
    fleet->addVehicle(std::make_unique<Bike>("BK-001", "Велосипед", 9));
    fleet->addVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4));
    fleet->addItem(std::make_unique<Helmet>("HL-001", "Шлем"));
    FleetReport report(fleet, BeginnerPolicy());

    EXPECT_EQ(report.totalCount(), 3u);
}

TEST(FleetReportTest, BikeAndHelmetConsumeNoEnergy) {
    auto fleet = std::make_shared<Fleet>();
    fleet->addVehicle(std::make_unique<Bike>("BK-001", "Велосипед", 9));
    fleet->addItem(std::make_unique<Helmet>("HL-001", "Шлем"));
    FleetReport report(fleet, BeginnerPolicy());

    EXPECT_DOUBLE_EQ(report.totalDailyEnergy(), 0.0);
}

TEST(FleetReportTest, SumsEnergyOfVehiclesAndItems) {
    auto fleet = std::make_shared<Fleet>();
    fleet->addVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4));
    fleet->addVehicle(std::make_unique<Bike>("BK-001", "Велосипед", 9));
    fleet->addItem(std::make_unique<DockStation>("DS-001", "Док-станция", 1.0));
    FleetReport report(fleet, BeginnerPolicy());

    EXPECT_DOUBLE_EQ(report.totalDailyEnergy(), 1.4);
}

TEST(FleetReportTest, InventoryListContainsVehiclesAndItems) {
    auto fleet = std::make_shared<Fleet>();
    fleet->addVehicle(std::make_unique<Bike>("BK-001", "Велосипед", 9));
    fleet->addItem(std::make_unique<Helmet>("HL-001", "Шлем"));
    FleetReport report(fleet, BeginnerPolicy());

    const auto list = report.inventoryList();
    ASSERT_EQ(list.size(), 2u);
    EXPECT_EQ(list[0]->inventoryNumber(), "BK-001");
    EXPECT_EQ(list[0]->name(), "Велосипед");
    EXPECT_EQ(list[1]->inventoryNumber(), "HL-001");
    EXPECT_EQ(list[1]->name(), "Шлем");
}

TEST(FleetReportTest, BeginnerVehiclesUsesPolicyThreshold) {
    auto fleet = std::make_shared<Fleet>();
    fleet->addVehicle(std::make_unique<Bike>("BK-005", "Сложный велосипед", 5));
    fleet->addVehicle(std::make_unique<Bike>("BK-006", "Простой велосипед", 6));
    FleetReport report(fleet, BeginnerPolicy(6));

    const auto beginners = report.beginnerVehicles();
    ASSERT_EQ(beginners.size(), 1u);
    EXPECT_EQ(beginners[0]->inventoryNumber(), "BK-006");
}

TEST(FleetReportTest, ReportSeesVehiclesAddedLater) {
    auto fleet = std::make_shared<Fleet>();
    FleetReport report(fleet, BeginnerPolicy());
    EXPECT_EQ(report.totalCount(), 0u);

    fleet->addVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4));

    EXPECT_EQ(report.totalCount(), 1u);
}
