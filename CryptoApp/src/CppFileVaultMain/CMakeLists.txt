cmake_minimum_required(VERSION 3.15)
project(CppFileVaultMain VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the path to your Qt6 installation
set(Qt6_DIR "C:/Qt/6.8.0/msvc2022_64/lib/cmake/Qt6")

# Add vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")  # Ensure all libraries are static

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(
    src/System
    src/logic
    src/gui
    src/DLLUtils
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

add_executable(CppFileVaultMain
    src/main.cpp
    src/StructUtils/StructUtils.cpp
    src/StructUtils/StructUtils.h
    src/GlobalDefinitions.h
    src/GlobalDefinitions.cpp
    src/System/SystemUtils.h
    src/System/SystemUtils.cpp
    src/HelperUtils/HelperUtils.cpp
    src/HelperUtils/HelperUtils.h
    src/DLLUtils/MasterDLLClass.h
    src/DLLUtils/RestApiDLL.cpp
    src/DLLUtils/RestApiDLL.h
    src/DLLUtils/CryptoDLL.cpp
    src/DLLUtils/CryptoDLL.h
    src/DLLUtils/FileMarkDLL.cpp
    src/DLLUtils/FileMarkDLL.h
    src/DLLUtils/MultiCryptoDLL.cpp
    src/DLLUtils/MultiCryptoDLL.h
    src/DLLUtils/FileScannerDLL.cpp
    src/DLLUtils/FileScannerDLL.h
    # Add other source files here
)

set_target_properties(CppFileVaultMain PROPERTIES
    AUTOMOC ON
)

# Specify the path to the cpprest_2_10.lib file
target_link_directories(CppFileVaultMain PRIVATE "C:/Users/prang/.vcpkg-clion/vcpkg/packages/cpprestsdk_x64-windows/lib")

target_link_libraries(CppFileVaultMain PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if (MSVC)
 target_compile_options(CppFileVaultMain PRIVATE /W4)
else ()
 target_compile_options(CppFileVaultMain PRIVATE -Wall -Wextra)
endif ()