cmake_minimum_required(VERSION 3.15)
project(SharedClasses)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(SharedClasses STATIC
    src/FileData.cpp
    src/FileData.h
    src/settingsClasses/Logs.cpp
    src/settingsClasses/Logs.h
    src/settingsClasses/CentralFileStorage.cpp
    src/settingsClasses/CentralFileStorage.h
    src/GlobalDefinitions.cpp
    src/GlobalDefinitions.h
)

target_include_directories(SharedClasses PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})