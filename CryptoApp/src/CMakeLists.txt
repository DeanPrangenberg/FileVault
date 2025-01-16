cmake_minimum_required(VERSION 3.15)
project(FileVaultRoot)

# Set the C++ standard globally for all projects
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Vcpkg as a toolchain (for static linking with libraries from vcpkg)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")  # Ensure all libraries are static

# Common output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Add subprojects
add_subdirectory(shared)  # Shared classes project
add_subdirectory(CppCryptoLib)  # Your cryptography module
add_subdirectory(CppFileVaultMain)  # Main application logic
add_subdirectory(CppFileMarkLib)  # File marking module
add_subdirectory(CppDirectoryScanner)  # Directory Scanner module

add_custom_target(RustFileCopy ALL
    COMMAND cargo build --release
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/../../RustFileCopy/target/release/RustFileCopy.exe ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RustFileCopy.exe
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../../RustFileCopy
    COMMENT "Building Rust FileCopy"
)

add_custom_target(GenDoc ALL
    COMMAND doxygen Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../../docs
    COMMENT "Generating documentation"
)

add_custom_target(GoRestApiWrapperLib ALL
    COMMAND go build -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/GoRestApiWrapperLib.dll -buildmode=c-shared .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/GoRestApiWrapperLib
    COMMENT "Building GO RestApiWrapper"
)

# Specify dependencies between projects
add_dependencies(CppFileVaultMain GenDoc)
add_dependencies(CppFileVaultMain RustFileCopy)
add_dependencies(CppFileVaultMain CppCryptoLib)
add_dependencies(CppFileVaultMain GoRestApiWrapperLib)
add_dependencies(CppFileVaultMain CppDirectoryScannerLib)
add_dependencies(CppFileVaultMain CppFileMarkLib)

# Compilation options for warnings
if (MSVC)
 add_compile_options(/W4 /GS)  # MSVC-specific warnings
else ()
 add_compile_options(-Wall -Wextra)  # GCC/Clang warnings
endif ()