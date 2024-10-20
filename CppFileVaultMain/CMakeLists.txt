cmake_minimum_required(VERSION 3.15)
project(CppFileVaultMain VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(
        src/System
        src/logic
        src/gui
        src/DLLUtils
)

add_executable(CppFileVaultMain
        src/main.cpp
        src/StructUtils/StructUtils.cpp
        src/StructUtils/StructUtils.h
        src/DLLUtils/DLLUtils.cpp
        src/DLLUtils/DLLUtils.h
        src/GlobalDefinitions.h
        src/GlobalDefinitions.cpp
        # Add other source files here
)

set_target_properties(CppFileVaultMain PROPERTIES
        AUTOMOC ON
)

target_link_libraries(CppFileVaultMain Qt6::Core Qt6::Gui Qt6::Widgets)

if (MSVC)
    target_compile_options(CppFileVaultMain PRIVATE /W4)
else ()
    target_compile_options(CppFileVaultMain PRIVATE -Wall -Wextra)
endif ()