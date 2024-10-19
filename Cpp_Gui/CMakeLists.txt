cmake_minimum_required(VERSION 3.15)
project(FileVault VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional: Set the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add Vcpkg as a toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static")

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Find static OpenSSL libraries
find_package(OpenSSL REQUIRED)

# Add source directories
include_directories(
    src/System
    src/logic
    src/gui
    src/FileVault
)

# Create the executable and ensure no console window opens
add_executable(FileVault
    src/main.cpp #[[
    src/FileScanner/FileUtils.h
    src/FileScanner/FileUtils.cpp
    src/gui/MainWindow.h
    src/gui/MainWindow.cpp
    src/gui/ui.cpp
    src/gui/functions.cpp
    src/System/SystemUtils.cpp
    src/System/SystemUtils.h]]
)

# Ensure Qt MOC is applied to header files
set_target_properties(FileVault PROPERTIES
    AUTOMOC ON
)

# Link Qt libraries and OpenSSL (statically)
target_link_libraries(FileVault Qt6::Core Qt6::Gui Qt6::Widgets OpenSSL::SSL OpenSSL::Crypto)

# Set output directories (already set in the root folder)

# Add version and other metadata
set_target_properties(FileVault PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# Force static linking
set(CMAKE_EXE_LINKER_FLAGS "-static")

# Add compile options (optional)
target_compile_options(FileVault PRIVATE -Wall -Wextra)