cmake_minimum_required(VERSION 3.15)
project(SharedClasses)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Add nlohmann::json package
find_package(nlohmann_json 3.11.3 REQUIRED)

add_library(SharedClasses STATIC
    src/FileData.cpp
    src/FileData.h
    src/settingsClasses/Logs.cpp
    src/settingsClasses/Logs.h
    src/settingsClasses/CentralFileStorage.cpp
    src/settingsClasses/CentralFileStorage.h
    src/GlobalDefinitions.cpp
    src/GlobalDefinitions.h
    src/settingsClasses/GetSettingsData.cpp
    src/settingsClasses/GetSettingsData.h
)

target_include_directories(SharedClasses PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Link nlohmann::json to your target
target_link_libraries(SharedClasses PUBLIC nlohmann_json::nlohmann_json)