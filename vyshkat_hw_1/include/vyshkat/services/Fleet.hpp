#pragma once

#include "vyshkat/domain/Item.hpp"
#include "vyshkat/domain/Vehicle.hpp"
#include <memory>
#include <vector>
#include <string>

namespace vyshkat {
class Fleet {
public:
    bool addVehicle(std::unique_ptr<Vehicle> vehicle);
    bool addItem(std::unique_ptr<Item> item);
    bool contains(const std::string &inventoryNumber) const;
    std::vector<const Vehicle *> vehicles() const;
    std::vector<const Item *> items() const;

private:
    std::vector<std::unique_ptr<Vehicle>> vehicles_;
    std::vector<std::unique_ptr<Item>> items_;
};
} // namespace vyshkat