cmake_minimum_required(VERSION 3.15)
project(RootProject)

# Set the C++ standard globally for all projects
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Vcpkg as a toolchain (if required)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static")

# Set the common build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Add the C++ subdirectories
add_subdirectory(Cpp_Cryptography)
add_subdirectory(Cpp_Gui)

# Set the dependency: FileVault depends on Cryptography
add_dependencies(FileVault Cryptograhpy)

# Compilation options
add_compile_options(-Wall -Wextra)

# Add Go DLL as a custom step
add_custom_target(build_go_encryptor
    COMMAND go build -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cryptoLib.dll -buildmode=c-shared .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Go_Multithreading
    DEPENDS Cryptograhpy  # Go DLL depends on C++ Cryptography
)

# Python: No compilation needed, but add dependency
add_custom_target(python_setup ALL
    COMMAND pip install -r ${CMAKE_SOURCE_DIR}/Python_Scripts/requirements.txt
)

# Add Rust Dropper, which packages everything together
add_custom_target(build_rust_dropper
    COMMAND cargo build --release
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Rust_Dropper
    DEPENDS build_go_encryptor FileVault
)

# Ensure dependencies are built in the correct order
add_dependencies(build_go_encryptor Cryptograhpy)
add_dependencies(build_rust_dropper FileVault build_go_encryptor)
add_dependencies(python_setup build_rust_dropper)