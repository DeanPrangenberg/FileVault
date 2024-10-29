cmake_minimum_required(VERSION 3.10)
project(CppSaveFileLib)

set(CMAKE_CXX_STANDARD 17)

# Add the nlohmann_json package
find_package(nlohmann_json REQUIRED)

# Enable building shared libraries
set(BUILD_SHARED_LIBS ON)

# Add the source files
add_library(CppSaveFileLib SHARED
		src/SaveFileOperations.cpp
		src/SaveFileOperations.h
		src/ConvertFunctions.cpp
		src/ConvertFunctions.cpp
		src/FileUtils.cpp
		src/FileUtils.cpp
		src/ExternFunctions.cpp
		src/ExternFunctions.cpp
)

if (MSVC)
	target_compile_options(CppCryptoLib PRIVATE /W4)
else ()
	target_compile_options(CppCryptoLib PRIVATE -Wall -Wextra)
endif ()

# Link the nlohmann_json library
target_link_libraries(CppSaveFileLib PRIVATE nlohmann_json::nlohmann_json)

# Define the export macro for DLL
target_compile_definitions(CppSaveFileLib PRIVATE CppSaveFileLib_EXPORTS SAVEFILELIB_EXPORTS)