#include "vyshkat/domain/Bike.hpp"


namespace vyshkat {
Bike::Bike(std::string inventoryNumber, std::string name, int beginnerSimplicity)
    : Vehicle(std::move(inventoryNumber), std::move(name), beginnerSimplicity) {};
}