#include <iostream>
#include <memory>

#include "vyshkat/di/Container.hpp"
#include "vyshkat/services/BeginnerPolicy.hpp"
#include "vyshkat/services/Fleet.hpp"
#include "vyshkat/services/FleetReport.hpp"
#include "vyshkat/services/FleetService.hpp"
#include "vyshkat/services/IServiceCenter.hpp"
#include "vyshkat/services/ServiceCenter.hpp"
#include "vyshkat/ui/ConsoleApp.hpp"

using namespace vyshkat;

int main() {
    Container container;
    container.registerSingleton<Fleet>([](Container &) { return std::make_shared<Fleet>(); });
    container.registerSingleton<IServiceCenter>([](Container &) { return std::make_shared<ServiceCenter>(); });
    container.registerSingleton<FleetService>([](Container &c) {
        return std::make_shared<FleetService>(c.resolve<Fleet>(), c.resolve<IServiceCenter>());
    });
    container.registerSingleton<FleetReport>([](Container &c) {
        return std::make_shared<FleetReport>(c.resolve<Fleet>(), BeginnerPolicy());
    });

    ConsoleApp app(container.resolve<FleetService>(), container.resolve<FleetReport>(), std::cin, std::cout);
    app.run();
    return 0;
}
