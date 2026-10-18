#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

#include "vyshkat/di/Container.hpp"
#include "vyshkat/domain/Scooter.hpp"
#include "vyshkat/services/Fleet.hpp"
#include "vyshkat/services/FleetService.hpp"
#include "vyshkat/services/IServiceCenter.hpp"
#include "vyshkat/services/ServiceCenter.hpp"

using vyshkat::Container;
using vyshkat::Fleet;
using vyshkat::FleetService;
using vyshkat::IServiceCenter;
using vyshkat::Scooter;
using vyshkat::ServiceCenter;

TEST(ContainerTest, ResolveReturnsSameInstance) {
    Container container;
    container.registerSingleton<Fleet>([](Container &) { return std::make_shared<Fleet>(); });

    auto first = container.resolve<Fleet>();
    auto second = container.resolve<Fleet>();

    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first, second);
}

TEST(ContainerTest, ResolveUnregisteredTypeThrows) {
    Container container;

    EXPECT_THROW(container.resolve<Fleet>(), std::runtime_error);
}

TEST(ContainerTest, FactoryIsCalledLazilyAndOnlyOnce) {
    Container container;
    int calls = 0;
    container.registerSingleton<Fleet>([&calls](Container &) {
        ++calls;
        return std::make_shared<Fleet>();
    });

    EXPECT_EQ(calls, 0);

    container.resolve<Fleet>();
    container.resolve<Fleet>();

    EXPECT_EQ(calls, 1);
}

TEST(ContainerTest, ResolvesInterfaceToImplementation) {
    Container container;
    container.registerSingleton<IServiceCenter>([](Container &) { return std::make_shared<ServiceCenter>(); });

    auto serviceCenter = container.resolve<IServiceCenter>();

    ASSERT_NE(serviceCenter, nullptr);
    EXPECT_NE(dynamic_cast<ServiceCenter *>(serviceCenter.get()), nullptr);
}

TEST(ContainerTest, FleetServiceSharesFleetWithContainer) {
    Container container;
    container.registerSingleton<Fleet>([](Container &) { return std::make_shared<Fleet>(); });
    container.registerSingleton<IServiceCenter>([](Container &) { return std::make_shared<ServiceCenter>(); });
    container.registerSingleton<FleetService>(
        [](Container &c) { return std::make_shared<FleetService>(c.resolve<Fleet>(), c.resolve<IServiceCenter>()); });

    auto service = container.resolve<FleetService>();
    auto result = service->acceptVehicle(std::make_unique<Scooter>("SC-001", "Самокат", 7, 0.4));

    EXPECT_TRUE(result.accepted);
    EXPECT_TRUE(container.resolve<Fleet>()->contains("SC-001"));
}
