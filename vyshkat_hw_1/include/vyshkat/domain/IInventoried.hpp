#pragma once
#include <string>

namespace vyshkat {
class IInventoried {
public:
    virtual std::string inventoryNumber() const = 0;
    virtual std::string name() const = 0;
    virtual ~IInventoried() = default;
};
} // namespace vyshkat