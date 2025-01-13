cmake_minimum_required(VERSION 3.29)
project(CppDirectoryScannerLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

# Enable building shared libraries
set(BUILD_SHARED_LIBS ON)

# Create the shared library
add_library(CppDirectoryScannerLib SHARED
        src/DirectoryScanner.cpp
        src/DirectoryScanner.h
		../CppFileVaultMain/src/GlobalDefinitions.cpp
    ../shared/FileData.cpp
    ../shared/FileData.h
    ../shared/settingsClasses/Logs.cpp
    ../shared/settingsClasses/Logs.h
)

# Compile options
if (MSVC)
    target_compile_options(CppDirectoryScannerLib PRIVATE /W4)
else ()
    target_compile_options(CppDirectoryScannerLib PRIVATE -Wall -Wextra)
endif ()

# Define the export macro for DLL
target_compile_definitions(CppDirectoryScannerLib PRIVATE CppDirectoryScannerLib_EXPORTS FILESCANNER_EXPORTS)