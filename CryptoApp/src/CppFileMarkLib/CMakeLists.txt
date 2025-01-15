cmake_minimum_required(VERSION 3.29)
project(CppFileMarkLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

# Enable building shared libraries
set(BUILD_SHARED_LIBS ON)

# Add the shared classes project if not already added
if (NOT TARGET SharedClasses)
  add_subdirectory(../shared ${CMAKE_BINARY_DIR}/shared_CppFileMarkLib)
endif ()

# Create the shared library
add_library(CppFileMarkLib SHARED
    src/FileMark.cpp
    src/FileMark.h
    ../shared/src/GlobalDefinitions.h
    ../shared/src/GlobalDefinitions.cpp
    ../shared/src/FileData.cpp
    ../shared/src/FileData.h
)

# Link the shared classes library
target_link_libraries(CppFileMarkLib PRIVATE SharedClasses)

# Compile options
if (MSVC)
  target_compile_options(CppFileMarkLib PRIVATE /W4)
else ()
  target_compile_options(CppFileMarkLib PRIVATE -Wall -Wextra)
endif ()

# Define the export macro for DLL
target_compile_definitions(CppFileMarkLib PRIVATE CppFileMarkLib_EXPORTS FILEMARKLIB_EXPORTS)