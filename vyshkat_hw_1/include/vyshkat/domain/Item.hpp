#pragma once

#include "vyshkat/domain/Asset.hpp"
#include <string>
#include <utility>

namespace vyshkat {


class Item : public Asset {
protected:
    Item(std::string inventoryNumber, std::string name) : Asset(std::move(inventoryNumber), std::move(name)) {}
};

} // namespace vyshkat