#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

#include "vyshkat/domain/Asset.hpp"

namespace {

class TestAsset : public vyshkat::Asset {
public:
    TestAsset(std::string number, std::string name) : Asset(std::move(number), std::move(name)) {}
};

} // namespace

TEST(AssetTest, StoresNumberAndName) {
    TestAsset asset("INV-001", "Шлем");
    EXPECT_EQ(asset.inventoryNumber(), "INV-001");
    EXPECT_EQ(asset.name(), "Шлем");
}


TEST(AssetTest, EmptyNumberThrows) { EXPECT_THROW(TestAsset("", "Шлем"), std::invalid_argument); }