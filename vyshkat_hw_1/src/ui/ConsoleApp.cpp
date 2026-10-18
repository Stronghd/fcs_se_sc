#include "vyshkat/ui/ConsoleApp.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "vyshkat/domain/Bike.hpp"
#include "vyshkat/domain/ChargingCabinet.hpp"
#include "vyshkat/domain/DockStation.hpp"
#include "vyshkat/domain/EBike.hpp"
#include "vyshkat/domain/Helmet.hpp"
#include "vyshkat/domain/Scooter.hpp"

namespace vyshkat {

ConsoleApp::ConsoleApp(std::shared_ptr<FleetService> service, std::shared_ptr<const FleetReport> report,
                       std::istream &in, std::ostream &out)
    : service_(std::move(service)), report_(std::move(report)), in_(in), out_(out) {
    registerCreators();
}

void ConsoleApp::run() {
    while (true) {
        printMenu();
        auto choice = readInt("Выберите пункт: ");
        if (!choice) {
            return;
        }

        bool inputOpen = true;
        switch (*choice) {
        case 1:
            inputOpen = acceptVehicle();
            break;
        case 2:
            inputOpen = addItem();
            break;
        case 3:
            printSummary();
            break;
        case 4:
            printBeginners();
            break;
        case 5:
            printInventory();
            break;
        case 0:
            out_ << "Конец программы\n";
            return;
        default:
            out_ << "Такого пункта нет\n";
            break;
        }
        if (!inputOpen) {
            return;
        }
    }
}

void ConsoleApp::printMenu() {
    out_ << "\n1. Принять транспорт\n"
            "2. Поставить вещь на баланс\n"
            "3. Отчёт: количество и энергия\n"
            "4. Транспорт для новичков\n"
            "5. Инвентарная ведомость\n"
            "0. Выход\n";
}

bool ConsoleApp::acceptVehicle() {
    auto kind = readLine("Вид (" + joinKeys(vehicleCreators_) + "): ");
    if (!kind) {
        return false;
    }
    auto creator = vehicleCreators_.find(*kind);
    if (creator == vehicleCreators_.end()) {
        out_ << "Неизвестный вид, доступно: " << joinKeys(vehicleCreators_) << "\n";
        return true;
    }

    try {
        auto vehicle = creator->second();
        if (!vehicle) {
            return false;
        }
        InspectionResult result = service_->acceptVehicle(std::move(vehicle));
        if (result.accepted) {
            out_ << "Принят\n";
        } else {
            out_ << "Отказ: " << result.reason << "\n";
        }
    } catch (const std::invalid_argument &e) {
        out_ << "Ошибка: " << e.what() << "\n";
    }
    return true;
}

bool ConsoleApp::addItem() {
    auto kind = readLine("Вид (" + joinKeys(itemCreators_) + "): ");
    if (!kind) {
        return false;
    }
    auto creator = itemCreators_.find(*kind);
    if (creator == itemCreators_.end()) {
        out_ << "Неизвестный вид, доступно: " << joinKeys(itemCreators_) << "\n";
        return true;
    }

    try {
        auto item = creator->second();
        if (!item) {
            return false;
        }
        InspectionResult result = service_->addItem(std::move(item));
        if (result.accepted) {
            out_ << "Поставлено на баланс\n";
        } else {
            out_ << "Отказ: " << result.reason << "\n";
        }
    } catch (const std::invalid_argument &e) {
        out_ << "Ошибка: " << e.what() << "\n";
    }
    return true;
}

void ConsoleApp::printSummary() {
    out_ << "Всего объектов: " << report_->totalCount() << "\n";
    out_ << "Энергия в сутки: " << report_->totalDailyEnergy() << " кВт·ч\n";
}

void ConsoleApp::printBeginners() {
    const auto vehicles = report_->beginnerVehicles();
    if (vehicles.empty()) {
        out_ << "Нет\n";
        return;
    }
    for (const Vehicle *vehicle : vehicles) {
        out_ << vehicle->inventoryNumber() << "  " << vehicle->name() << "\n";
    }
}

void ConsoleApp::printInventory() {
    const auto list = report_->inventoryList();
    if (list.empty()) {
        out_ << "Нет\n";
        return;
    }
    for (const IInventoried *entry : list) {
        out_ << entry->inventoryNumber() << "  " << entry->name() << "\n";
    }
}

std::optional<std::string> ConsoleApp::readLine(const std::string &prompt) {
    out_ << prompt;
    std::string line;
    if (!std::getline(in_, line)) {
        return std::nullopt;
    }
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    return line;
}

std::optional<int> ConsoleApp::readInt(const std::string &prompt) {
    while (true) {
        auto line = readLine(prompt);
        if (!line) {
            return std::nullopt;
        }
        try {
            std::size_t pos = 0;
            int value = std::stoi(*line, &pos);
            if (pos == line->size()) {
                return value;
            }
        } catch (const std::exception &) {
        }
        out_ << "Введите целое число\n";
    }
}

std::optional<double> ConsoleApp::readDouble(const std::string &prompt) {
    while (true) {
        auto line = readLine(prompt);
        if (!line) {
            return std::nullopt;
        }
        try {
            std::size_t pos = 0;
            double value = std::stod(*line, &pos);
            if (pos == line->size()) {
                return value;
            }
        } catch (const std::exception &) {
        }
        out_ << "Введите число, например 0.4\n";
    }
}

void ConsoleApp::registerCreators() {
    vehicleCreators_["scooter"] = [this]() -> std::unique_ptr<Vehicle> {
        auto number = readLine("Инвентарный номер: ");
        auto name = number ? readLine("Название: ") : std::nullopt;
        auto simplicity = name ? readInt("Простота для новичка (1-10): ") : std::nullopt;
        auto energy = simplicity ? readDouble("Расход кВт·ч в сутки: ") : std::nullopt;
        if (!energy) {
            return nullptr;
        }
        return std::make_unique<Scooter>(*number, *name, *simplicity, *energy);
    };
    vehicleCreators_["ebike"] = [this]() -> std::unique_ptr<Vehicle> {
        auto number = readLine("Инвентарный номер: ");
        auto name = number ? readLine("Название: ") : std::nullopt;
        auto simplicity = name ? readInt("Простота для новичка (1-10): ") : std::nullopt;
        auto energy = simplicity ? readDouble("Расход кВт·ч в сутки: ") : std::nullopt;
        if (!energy) {
            return nullptr;
        }
        return std::make_unique<EBike>(*number, *name, *simplicity, *energy);
    };
    vehicleCreators_["bike"] = [this]() -> std::unique_ptr<Vehicle> {
        auto number = readLine("Инвентарный номер: ");
        auto name = number ? readLine("Название: ") : std::nullopt;
        auto simplicity = name ? readInt("Простота для новичка (1-10): ") : std::nullopt;
        if (!simplicity) {
            return nullptr;
        }
        return std::make_unique<Bike>(*number, *name, *simplicity);
    };

    itemCreators_["helmet"] = [this]() -> std::unique_ptr<Item> {
        auto number = readLine("Инвентарный номер: ");
        auto name = number ? readLine("Название: ") : std::nullopt;
        if (!name) {
            return nullptr;
        }
        return std::make_unique<Helmet>(*number, *name);
    };
    itemCreators_["dock"] = [this]() -> std::unique_ptr<Item> {
        auto number = readLine("Инвентарный номер: ");
        auto name = number ? readLine("Название: ") : std::nullopt;
        auto energy = name ? readDouble("Расход кВт·ч в сутки: ") : std::nullopt;
        if (!energy) {
            return nullptr;
        }
        return std::make_unique<DockStation>(*number, *name, *energy);
    };
    itemCreators_["cabinet"] = [this]() -> std::unique_ptr<Item> {
        auto number = readLine("Инвентарный номер: ");
        auto name = number ? readLine("Название: ") : std::nullopt;
        auto energy = name ? readDouble("Расход кВт·ч в сутки: ") : std::nullopt;
        if (!energy) {
            return nullptr;
        }
        return std::make_unique<ChargingCabinet>(*number, *name, *energy);
    };
}

} // namespace vyshkat
