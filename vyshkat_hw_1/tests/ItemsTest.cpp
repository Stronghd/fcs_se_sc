#include <gtest/gtest.h>

#include <stdexcept>

#include "vyshkat/domain/ChargingCabinet.hpp"
#include "vyshkat/domain/DockStation.hpp"
#include "vyshkat/domain/Helmet.hpp"
#include "vyshkat/domain/IEnergyConsumer.hpp"
#include "vyshkat/domain/Item.hpp"

using vyshkat::ChargingCabinet;
using vyshkat::DockStation;
using vyshkat::Helmet;
using vyshkat::IEnergyConsumer;
using vyshkat::Item;

TEST(HelmetTest, StoresNumberAndName) {
    Helmet helmet("HL-001", "Шлем");
    EXPECT_EQ(helmet.inventoryNumber(), "HL-001");
    EXPECT_EQ(helmet.name(), "Шлем");
}

TEST(HelmetTest, IsNotEnergyConsumer) {
    Helmet helmet("HL-002", "Шлем");
    const Item &item = helmet;
    EXPECT_EQ(dynamic_cast<const IEnergyConsumer *>(&item), nullptr);
}

TEST(DockStationTest, StoresDailyEnergy) {
    DockStation dock("DS-001", "Док-станция", 0.8);
    EXPECT_EQ(dock.inventoryNumber(), "DS-001");
    EXPECT_DOUBLE_EQ(dock.dailyEnergyKwh(), 0.8);
}

TEST(DockStationTest, RejectsNegativeEnergy) {
    EXPECT_THROW(DockStation("DS-002", "Док-станция", -0.5), std::invalid_argument);
}

TEST(ChargingCabinetTest, StoresDailyEnergy) {
    ChargingCabinet cabinet("CC-001", "Зарядный шкаф", 2.5);
    EXPECT_DOUBLE_EQ(cabinet.dailyEnergyKwh(), 2.5);
}

TEST(ChargingCabinetTest, RejectsNegativeEnergy) {
    EXPECT_THROW(ChargingCabinet("CC-002", "Зарядный шкаф", -1.0), std::invalid_argument);
}

TEST(ItemsTest, DockStationAndCabinetAreEnergyConsumers) {
    DockStation dock("DS-003", "Док-станция", 0.8);
    ChargingCabinet cabinet("CC-003", "Зарядный шкаф", 2.5);
    const Item &dockAsItem = dock;
    const Item &cabinetAsItem = cabinet;
    EXPECT_NE(dynamic_cast<const IEnergyConsumer *>(&dockAsItem), nullptr);
    EXPECT_NE(dynamic_cast<const IEnergyConsumer *>(&cabinetAsItem), nullptr);
}
