#pragma once
#include "vyshkat/domain/IInventoried.hpp"


namespace vyshkat {
class Asset : public IInventoried {
public:
    std::string inventoryNumber() const override { return inventoryNumber_; }
    std::string name() const override { return name_; }

protected:
    Asset(std::string inventoryNumber, std::string name);

private:
    std::string inventoryNumber_;
    std::string name_;
};

} // namespace vyshkat