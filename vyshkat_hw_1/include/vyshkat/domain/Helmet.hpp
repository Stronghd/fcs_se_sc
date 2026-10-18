#pragma once

#include "vyshkat/domain/Item.hpp"
#include <utility>
#include <string>

namespace vyshkat {
class Helmet : public Item {
public:
    Helmet(std::string inventoryNumber, std::string name) : Item(std::move(inventoryNumber), std::move(name)) {}
};
} // namespace vyshkat