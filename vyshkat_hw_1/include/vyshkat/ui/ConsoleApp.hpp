#pragma once

#include <functional>
#include <istream>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include "vyshkat/domain/Item.hpp"
#include "vyshkat/domain/Vehicle.hpp"
#include "vyshkat/services/FleetReport.hpp"
#include "vyshkat/services/FleetService.hpp"

namespace vyshkat {

class ConsoleApp {
public:
    ConsoleApp(std::shared_ptr<FleetService> service, std::shared_ptr<const FleetReport> report,
               std::istream &in, std::ostream &out);
    void run();

private:
    using VehicleCreator = std::function<std::unique_ptr<Vehicle>()>;
    using ItemCreator = std::function<std::unique_ptr<Item>()>;

    void printMenu();
    bool acceptVehicle();
    bool addItem();
    void printSummary();
    void printBeginners();
    void printInventory();


    std::optional<std::string> readLine(const std::string &prompt);
    std::optional<int> readInt(const std::string &prompt);
    std::optional<double> readDouble(const std::string &prompt);

    void registerCreators();

    template <class Creators>
    static std::string joinKeys(const Creators &creators) {
        std::string result;
        for (const auto &[key, creator] : creators) {
            result += result.empty() ? key : ", " + key;
        }
        return result;
    }

    std::shared_ptr<FleetService> service_;
    std::shared_ptr<const FleetReport> report_;
    std::istream &in_;
    std::ostream &out_;
    std::map<std::string, VehicleCreator> vehicleCreators_;
    std::map<std::string, ItemCreator> itemCreators_;
};

}
