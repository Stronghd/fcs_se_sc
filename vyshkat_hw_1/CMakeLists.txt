cmake_minimum_required(VERSION 3.20)

project(vyshkat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(vyshkat_core src/domain/Asset.cpp
                                src/domain/Vehicle.cpp
                                src/domain/Scooter.cpp
                                src/domain/EBike.cpp
                                src/domain/Bike.cpp
                                src/domain/DockStation.cpp
                                src/domain/ChargingCabinet.cpp
                                src/services/ServiceCenter.cpp
                                src/services/BeginnerPolicy.cpp
                                src/services/Fleet.cpp
                                src/services/FleetService.cpp
                                src/services/FleetReport.cpp
                                src/ui/ConsoleApp.cpp)
target_include_directories(vyshkat_core PUBLIC include)

add_executable(vyshkat app/main.cpp)
target_link_libraries(vyshkat PRIVATE vyshkat_core)

include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(vyshkat_tests tests/AssetTest.cpp
                             tests/VehicleTest.cpp
                             tests/VehicleKindsTest.cpp
                             tests/ItemsTest.cpp
                             tests/ServiceCenterTest.cpp
                             tests/BeginnerPolicyTest.cpp
                             tests/FleetTest.cpp
                             tests/FleetReportTest.cpp
                             tests/ContainerTest.cpp
                             tests/ConsoleAppTest.cpp
                             tests/FleetServiceTest.cpp)
target_include_directories(vyshkat_tests PRIVATE tests)
target_link_libraries(vyshkat_tests PRIVATE vyshkat_core GTest::gtest_main GTest::gmock)
include(GoogleTest)
gtest_discover_tests(vyshkat_tests)

option(ENABLE_COVERAGE "Build with coverage" OFF)
if(ENABLE_COVERAGE)
  target_compile_options(vyshkat_core PRIVATE --coverage)
  target_link_options(vyshkat_core PUBLIC --coverage)
  target_compile_options(vyshkat_tests PRIVATE --coverage)
endif()