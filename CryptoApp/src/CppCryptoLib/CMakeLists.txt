cmake_minimum_required(VERSION 3.29)
project(CppCryptoLib)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkgNew/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

# Enable building shared libraries
set(BUILD_SHARED_LIBS ON)

# Find OpenSSL library
find_package(OpenSSL REQUIRED)

# Add the shared classes project if not already added
if (NOT TARGET SharedClasses)
  add_subdirectory(../shared ${CMAKE_BINARY_DIR}/shared_CppCryptoLib)
endif()

# Create the shared library
add_library(CppCryptoLib SHARED
  src/alogs/AES128.cpp
  src/alogs/AES256.cpp
  src/alogs/KeyGen.cpp
  src/api/CryptoAPI.cpp
  src/api/CryptoAPI.h
  src/alogs/SHA512.cpp
  src/Helper/CryptoHelperUtils.cpp
  src/Helper/CryptoHelperUtils.h
  src/alogs/AES128.h
  src/alogs/AES256.h
  src/alogs/SHA512.h
  src/alogs/KeyGen.h
  ../shared/src/GlobalDefinitions.h
  ../shared/src/GlobalDefinitions.cpp
  ../shared/src/FileData.cpp
  ../shared/src/FileData.h
    src/alogs/BLAKE2.cpp
    src/alogs/BLAKE2.h
)

# Link OpenSSL and shared classes libraries
target_link_libraries(CppCryptoLib PRIVATE OpenSSL::SSL OpenSSL::Crypto SharedClasses)

# Compile options for warnings
if (MSVC)
 target_compile_options(CppCryptoLib PRIVATE /W4)
else ()
 target_compile_options(CppCryptoLib PRIVATE -Wall -Wextra)
endif()

# Define the export macro for DLL
target_compile_definitions(CppCryptoLib PRIVATE CppCryptoLib_EXPORTS CRYPTOLIBRARY_EXPORTS)