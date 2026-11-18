cmake_minimum_required(VERSION 3.16)
project(kpo_gradebook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ICU REQUIRED COMPONENTS uc)

add_library(kpo_domain STATIC
    src/domain/Criterion.cpp
    src/domain/Evaluation.cpp
    src/domain/FinalGradeService.cpp
    src/domain/Gradebook.cpp
    src/domain/Rubric.cpp
    src/domain/Student.cpp
)
target_include_directories(kpo_domain PUBLIC src/domain)
target_link_libraries(kpo_domain PRIVATE ICU::uc)

add_library(kpo_application STATIC src/application/AssessmentFacade.cpp)
target_include_directories(kpo_application PUBLIC src/application)
target_link_libraries(kpo_application PUBLIC kpo_domain)

add_library(kpo_adapters STATIC
    src/adapters/console/ConsoleApp.cpp
    src/adapters/export/FileResultExporter.cpp
    src/adapters/memory/InMemoryEvaluationRepository.cpp
    src/adapters/memory/StudentRegistry.cpp
)
target_include_directories(kpo_adapters PUBLIC
    src/adapters/console
    src/adapters/export
    src/adapters/memory
)
target_link_libraries(kpo_adapters PUBLIC kpo_application)

add_executable(kpo src/composition/main.cpp)
target_link_libraries(kpo PRIVATE kpo_adapters)

foreach(target kpo_domain kpo_application kpo_adapters kpo)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()
