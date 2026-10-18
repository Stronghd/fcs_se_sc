#include <stdexcept>
#include <utility>

#include "vyshkat/domain/Asset.hpp"

vyshkat::Asset::Asset(std::string inventoryNumber, std::string name)
    : inventoryNumber_(std::move(inventoryNumber)), name_(std::move(name)) {
    if (inventoryNumber_.empty()) {
        throw std::invalid_argument("Inventory number must be not empty");
    }
}