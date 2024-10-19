cmake_minimum_required(VERSION 3.29)
project(Cryptograhpy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static")

# Enable building shared libraries
set(BUILD_SHARED_LIBS ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Create the shared library
add_library(Cryptograhpy SHARED
    src/alogs/AES128.cpp
    src/alogs/AES256.cpp
    src/alogs/KeyGen.cpp
    src/CryptoNamespace.cpp
    src/CryptoNamespace.h
)

# Link OpenSSL libraries
target_link_libraries(Cryptograhpy PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Set properties for output directories (already set in the root folder)

# Compile options
target_compile_options(Cryptograhpy PRIVATE -Wall -Wextra)

# Define the export macro for DLL
target_compile_definitions(Cryptograhpy PRIVATE Cryptograhpy_EXPORTS CRYPTOLIBRARY_EXPORTS)