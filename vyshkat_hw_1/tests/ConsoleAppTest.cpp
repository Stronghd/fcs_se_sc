#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>

#include "vyshkat/services/BeginnerPolicy.hpp"
#include "vyshkat/services/Fleet.hpp"
#include "vyshkat/services/FleetReport.hpp"
#include "vyshkat/services/FleetService.hpp"
#include "vyshkat/services/ServiceCenter.hpp"
#include "vyshkat/ui/ConsoleApp.hpp"

using vyshkat::BeginnerPolicy;
using vyshkat::ConsoleApp;
using vyshkat::Fleet;
using vyshkat::FleetReport;
using vyshkat::FleetService;
using vyshkat::ServiceCenter;

namespace {

struct ConsoleFixture {
    std::shared_ptr<Fleet> fleet = std::make_shared<Fleet>();
    std::istringstream in;
    std::ostringstream out;

    explicit ConsoleFixture(const std::string &input) : in(input) {}

    void run() {
        auto service = std::make_shared<FleetService>(fleet, std::make_shared<ServiceCenter>());
        auto report = std::make_shared<FleetReport>(fleet, BeginnerPolicy());
        ConsoleApp app(service, report, in, out);
        app.run();
    }

    bool printed(const std::string &text) const { return out.str().find(text) != std::string::npos; }
};

} // namespace

TEST(ConsoleAppTest, ExitsOnZero) {
    ConsoleFixture console("0\n");
    console.run();

    EXPECT_TRUE(console.printed("Конец программы"));
}

TEST(ConsoleAppTest, StopsOnEmptyInput) {
    ConsoleFixture console("");
    console.run();

    EXPECT_TRUE(console.fleet->vehicles().empty());
}

TEST(ConsoleAppTest, AsksAgainOnLettersInsteadOfNumber) {
    ConsoleFixture console("abc\n\n12abc\n0\n");
    console.run();

    EXPECT_TRUE(console.printed("Введите целое число"));
    EXPECT_TRUE(console.printed("Конец программы"));
}

TEST(ConsoleAppTest, ReportsUnknownMenuItem) {
    ConsoleFixture console("9\n0\n");
    console.run();

    EXPECT_TRUE(console.printed("Такого пункта нет"));
}

TEST(ConsoleAppTest, AcceptsScooterAndShowsItInInventory) {
    ConsoleFixture console("1\nscooter\nSC-001\nСамокат\n7\n0.4\n5\n0\n");
    console.run();

    EXPECT_TRUE(console.printed("Принят"));
    EXPECT_TRUE(console.printed("SC-001  Самокат"));
    EXPECT_EQ(console.fleet->vehicles().size(), 1u);
}

TEST(ConsoleAppTest, ShowsRejectionReasonFromServiceCenter) {
    ConsoleFixture console("1\nscooter\nSC-002\nМощный самокат\n7\n2.0\n0\n");
    console.run();

    EXPECT_TRUE(console.printed("Отказ"));
    EXPECT_TRUE(console.fleet->vehicles().empty());
}

TEST(ConsoleAppTest, ReportsErrorForSimplicityOutOfRange) {
    ConsoleFixture console("1\nbike\nBK-001\nВелосипед\n11\n0\n");
    console.run();

    EXPECT_TRUE(console.printed("Ошибка"));
    EXPECT_TRUE(console.fleet->vehicles().empty());
}

TEST(ConsoleAppTest, ReportsUnknownKind) {
    ConsoleFixture console("1\nrocket\n0\n");
    console.run();

    EXPECT_TRUE(console.printed("Неизвестный вид"));
}

TEST(ConsoleAppTest, StopsWhenInputEndsInTheMiddleOfVehicle) {
    ConsoleFixture console("1\nscooter\nSC-001\n");
    console.run();

    EXPECT_TRUE(console.fleet->vehicles().empty());
}

TEST(ConsoleAppTest, AddsItemAndPrintsSummary) {
    ConsoleFixture console("2\ndock\nDS-001\nДок-станция\n1.0\n3\n0\n");
    console.run();

    EXPECT_TRUE(console.printed("Поставлено на баланс"));
    EXPECT_TRUE(console.printed("Всего объектов: 1"));
    EXPECT_TRUE(console.printed("Энергия в сутки: 1"));
}
