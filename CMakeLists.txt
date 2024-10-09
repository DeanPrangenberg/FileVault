cmake_minimum_required(VERSION 3.15)
project(Ransomware VERSION 1.0 LANGUAGES CXX)

# Setze den C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional: Setze den Build-Typ
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
endif()

# Füge Vcpkg als Toolchain hinzu
set(CMAKE_TOOLCHAIN_FILE "C:/Users/prang/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static")

# Finde Qt-Pakete (stelle sicher, dass du statische Qt-Bibliotheken verwendest)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Finde statische OpenSSL-Bibliotheken
find_package(OpenSSL REQUIRED)

# Füge die Quellverzeichnisse hinzu
include_directories(
src/utils
src/logic
src/gui
)

# Setze die Quell- und Header-Dateien
set(SOURCES
src/main.cpp
src/gui/MainWindow.cpp
src/utils/SystemUtils.cpp
src/utils/FileUtils.cpp
        src/utils/KeyUtils.cpp
src/logic/encryptor.cpp
src/logic/decryptor.cpp
src/utils/CryptoAes128.cpp
src/utils/CryptoAes256.cpp
src/gui/ui.cpp
src/gui/functions.cpp
)

set(HEADERS
src/gui/MainWindow.h
src/utils/SystemUtils.h
src/utils/FileUtils.h
        src/utils/KeyUtils.h
src/logic/encryptor.h
src/logic/decryptor.h
src/utils/CryptoAes128.h
src/utils/CryptoAes256.h
)

# Erstelle das Executable und stelle sicher, dass kein Konsolenfenster geöffnet wird
add_executable(Ransomware WIN32 ${SOURCES} ${HEADERS})

# Mache sicher, dass Qt MOC auf den Header-Dateien angewendet wird
set_target_properties(Ransomware PROPERTIES
AUTOMOC ON
)

# Verlinke die Qt-Bibliotheken und OpenSSL (statisch)
target_link_libraries(Ransomware Qt6::Core Qt6::Gui Qt6::Widgets OpenSSL::SSL OpenSSL::Crypto)

# Setze die Ausgabeverzeichnisse
set_target_properties(Ransomware PROPERTIES
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Füge die Version und andere Metadaten hinzu
set_target_properties(Ransomware PROPERTIES
VERSION ${PROJECT_VERSION}
SOVERSION 1
)

# Statische Verlinkung erzwingen
set(CMAKE_EXE_LINKER_FLAGS "-static")

# Füge Kompilierungsoptionen hinzu (optional)
target_compile_options(Ransomware PRIVATE -Wall -Wextra)