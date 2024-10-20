cmake_minimum_required(VERSION 3.10)
project(CppSaveFileLib)

set(CMAKE_CXX_STANDARD 17)

# Add the nlohmann_json package
find_package(nlohmann_json REQUIRED)

# Add the source files
add_library(CppSaveFileLib SHARED SaveFileOperations.cpp)

# Link the nlohmann_json library
target_link_libraries(CppSaveFileLib PRIVATE nlohmann_json::nlohmann_json)