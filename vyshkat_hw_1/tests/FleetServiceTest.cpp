#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "mocks/MockServiceCenter.hpp"
#include "vyshkat/di/Container.hpp"
#include "vyshkat/domain/Bike.hpp"
#include "vyshkat/domain/Helmet.hpp"
#include "vyshkat/domain/Scooter.hpp"
#include "vyshkat/services/Fleet.hpp"
#include "vyshkat/services/FleetService.hpp"
#include "vyshkat/services/IServiceCenter.hpp"
#include "vyshkat/services/ServiceCenter.hpp"

using ::testing::_;
using ::testing::Property;
using ::testing::Return;
using vyshkat::Bike;
using vyshkat::Container;
using vyshkat::Fleet;
using vyshkat::FleetService;
using vyshkat::Helmet;
using vyshkat::InspectionResult;
using vyshkat::IServiceCenter;
using vyshkat::MockServiceCenter;
using vyshkat::Scooter;
using vyshkat::ServiceCenter;
using vyshkat::Vehicle;

namespace {

Container makeTestContainer(const std::shared_ptr<MockServiceCenter> &mock) {
    Container container;
    container.registerSingleton<Fleet>([](Container &) { return std::make_shared<Fleet>(); });
    container.registerSingleton<IServiceCenter>([mock](Container &) { return mock; });
    container.registerSingleton<FleetService>(
        [](Container &c) { return std::make_shared<FleetService>(c.resolve<Fleet>(), c.resolve<IServiceCenter>()); });
    return container;
}

} // namespace

TEST(FleetServiceTest, AddsVehicleWhenMockAccepts) {
    auto mock = std::make_shared<MockServiceCenter>();
    EXPECT_CALL(*mock, inspect(_)).WillOnce(Return(InspectionResult{true, ""}));
    Container container = makeTestContainer(mock);

    auto result = container.resolve<FleetService>()->acceptVehicle(std::make_unique<Bike>("BK-001", "Велосипед", 9));

    EXPECT_TRUE(result.accepted);
    EXPECT_TRUE(container.resolve<Fleet>()->contains("BK-001"));
}

TEST(FleetServiceTest, RejectsVehicleWhenMockRejects) {
    auto mock = std::make_shared<MockServiceCenter>();
    EXPECT_CALL(*mock, inspect(_)).WillOnce(Return(InspectionResult{false, "тормоза неисправны"}));
    Container container = makeTestContainer(mock);

    auto result = container.resolve<FleetService>()->acceptVehicle(std::make_unique<Bike>("BK-001", "Велосипед", 9));

    EXPECT_FALSE(result.accepted);
    EXPECT_EQ(result.reason, "тормоза неисправны");
    EXPECT_TRUE(container.resolve<Fleet>()->vehicles().empty());
}

TEST(FleetServiceTest, PassesTheSameVehicleToInspection) {
    auto mock = std::make_shared<MockServiceCenter>();
    EXPECT_CALL(*mock, inspect(Property(&Vehicle::inventoryNumber, "SC-001")))
        .WillOnce(Return(InspectionResult{true, ""}));
    Container container = makeTestContainer(mock);

    container.resolve<FleetService>()->acceptVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4));
}

TEST(FleetServiceTest, RejectsDuplicateAfterSuccessfulInspection) {
    auto mock = std::make_shared<MockServiceCenter>();
    EXPECT_CALL(*mock, inspect(_)).Times(2).WillRepeatedly(Return(InspectionResult{true, ""}));
    Container container = makeTestContainer(mock);
    auto service = container.resolve<FleetService>();

    service->acceptVehicle(std::make_unique<Bike>("BK-001", "Велосипед", 9));
    auto second = service->acceptVehicle(std::make_unique<Bike>("BK-001", "Другой велосипед", 8));

    EXPECT_FALSE(second.accepted);
    EXPECT_FALSE(second.reason.empty());
    EXPECT_EQ(container.resolve<Fleet>()->vehicles().size(), 1u);
}

TEST(FleetServiceTest, ItemsDoNotGoThroughInspection) {
    auto mock = std::make_shared<MockServiceCenter>();
    EXPECT_CALL(*mock, inspect(_)).Times(0);
    Container container = makeTestContainer(mock);

    auto result = container.resolve<FleetService>()->addItem(std::make_unique<Helmet>("HL-001", "Шлем"));

    EXPECT_TRUE(result.accepted);
    EXPECT_TRUE(container.resolve<Fleet>()->contains("HL-001"));
}

TEST(FleetServiceTest, RejectsDuplicateItem) {
    auto mock = std::make_shared<MockServiceCenter>();
    Container container = makeTestContainer(mock);
    auto service = container.resolve<FleetService>();

    service->addItem(std::make_unique<Helmet>("HL-001", "Шлем"));
    auto second = service->addItem(std::make_unique<Helmet>("HL-001", "Шлем L"));

    EXPECT_FALSE(second.accepted);
    EXPECT_EQ(container.resolve<Fleet>()->items().size(), 1u);
}

TEST(FleetServiceTest, RealServiceCenterRejectsHighEnergyScooter) {
    auto fleet = std::make_shared<Fleet>();
    FleetService service(fleet, std::make_shared<ServiceCenter>());

    auto result = service.acceptVehicle(std::make_unique<Scooter>("SC-002", "Мощный самокат", 7, 2.0));

    EXPECT_FALSE(result.accepted);
    EXPECT_TRUE(fleet->vehicles().empty());
}
