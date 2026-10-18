#pragma once

#include "vyshkat/domain/Asset.hpp"
#include <string>
#include <utility>

namespace vyshkat {
class Vehicle : public Asset {
public:
    int beginnerSimplicity() const;

protected:
    Vehicle(std::string inventoryNumber, std::string name, int beginnerSimplicity);


private:
    int beginnerSimplicity_;
};
} // namespace vyshkat