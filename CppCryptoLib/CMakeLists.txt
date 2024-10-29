cmake_minimum_required(VERSION 3.29)
project(CppCryptoLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

# Enable building shared libraries
set(BUILD_SHARED_LIBS ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Create the shared library
add_library(CppCryptoLib SHARED
		src/alogs/AES128.cpp
		src/alogs/AES256.cpp
		src/alogs/KeyGen.cpp
		src/CryptoNamespace.cpp
		src/CryptoNamespace.h
		src/alogs/SHA512.cpp
		../CppFileVaultMain/src/GlobalDefinitions.h
		../CppFileVaultMain/src/GlobalDefinitions.cpp
		src/HelperUtils.cpp
		src/HelperUtils.cpp
)

# Link OpenSSL libraries
target_link_libraries(CppCryptoLib PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Compile options
if (MSVC)
	target_compile_options(CppCryptoLib PRIVATE /W4)
else ()
	target_compile_options(CppCryptoLib PRIVATE -Wall -Wextra)
endif ()

# Define the export macro for DLL
target_compile_definitions(CppCryptoLib PRIVATE CppCryptoLib_EXPORTS CRYPTOLIBRARY_EXPORTS)