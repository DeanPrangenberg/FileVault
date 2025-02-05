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

# Find the required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Include directories for the project
include_directories(
    src/System
    src/logic
    src/gui
    src/DLLUtils
)

# Add this section before the add_executable command
qt_add_resources(CppFileVaultMain_resources res/resources.qrc)

# Custom command to always compile the .qrc file
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qrc_resources.cpp
    COMMAND ${CMAKE_COMMAND} -E echo "Compiling resources.qrc"
    COMMAND ${Qt6_DIR}/../../../bin/rcc -name resources -o ${CMAKE_CURRENT_BINARY_DIR}/qrc_resources.cpp ${CMAKE_CURRENT_SOURCE_DIR}/res/resources.qrc
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res/resources.qrc
    COMMENT "Always compiling resources.qrc"
)

# Custom target to ensure the custom command is run
add_custom_target(compile_resources ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qrc_resources.cpp
)

# Add the shared classes project if not already added
if (NOT TARGET SharedClasses)
  add_subdirectory(../shared ${CMAKE_BINARY_DIR}/shared_CppFileVaultMain)
endif()

# Define the main executable target
add_executable(CppFileVaultMain
    src/main.cpp
    src/AttributeUtils/AttributeUtils.cpp
    src/AttributeUtils/AttributeUtils.h
    src/System/SystemUtils.h
    src/System/SystemUtils.cpp
    src/HelperUtils/HelperUtils.cpp
    src/HelperUtils/HelperUtils.h
    src/DLLUtils/MasterDLLClass.h
    src/DLLUtils/CryptoDLL.cpp
    src/DLLUtils/CryptoDLL.h
    src/DLLUtils/FileMarkDLL.cpp
    src/DLLUtils/FileMarkDLL.h
    src/DLLUtils/FileScannerDLL.cpp
    src/DLLUtils/FileScannerDLL.h
    src/gui/FileVaultGui.cpp
    src/gui/FileVaultGui.h
    ${CMAKE_CURRENT_BINARY_DIR}/qrc_resources.cpp
    res/app_icon.rc
    src/gui/styleClass/StyleSetter.h
    src/gui/styleClass/StyleSetter.tpp
    src/gui/widgetClasses/Screens/DecryptionScreenWidget.cpp
    src/gui/widgetClasses/Screens/DecryptionScreenWidget.h
    src/gui/widgetClasses/Screens/EncryptionScreenWidget.cpp
    src/gui/widgetClasses/Screens/EncryptionScreenWidget.h
    src/gui/widgetClasses/Screens/SettingsScreenWidget.cpp
    src/gui/widgetClasses/Screens/SettingsScreenWidget.h
    src/gui/widgetClasses/Screens/StatisticsScreenWidget.cpp
    src/gui/widgetClasses/Screens/StatisticsScreenWidget.h
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/MasterSettingsWidget.h
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/MasterSettingsWidget.cpp
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/FileDeletionWidget.cpp
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/FileDeletionWidget.h
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/StandardAlgorithmWidget.cpp
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/StandardAlgorithmWidget.h
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/LogsLocationWidget.cpp
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/LogsLocationWidget.h
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/NewPasswordWidget.cpp
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/NewPasswordWidget.h
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/LanguageSelectionWidget.cpp
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/LanguageSelectionWidget.h
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/DatabaseManagementWidget.cpp
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/DatabaseManagementWidget.h
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/CentralFileStorageWidget.cpp
    src/gui/widgetClasses/elementWidgets/SettingsWidgets/CentralFileStorageWidget.h
    src/gui/widgetClasses/elementWidgets/smallElements/PasswordLineEdit.h
    src/gui/styleClass/StyleSetter.cpp
    src/gui/widgetClasses/elementWidgets/cryptionWidgets/FilePicker.h
    src/gui/widgetClasses/elementWidgets/cryptionWidgets/FilePicker.cpp
    src/gui/widgetClasses/elementWidgets/smallElements/LoadingWindow.cpp
    src/gui/widgetClasses/elementWidgets/statisticWidgets/CakeDiagram.h
    src/gui/widgetClasses/elementWidgets/statisticWidgets/CakeDiagram.cpp
    src/gui/widgetClasses/elementWidgets/statisticWidgets/AlgorithmStatisticWidget.cpp
    src/DLLUtils/RestAPI/converter.cpp
    src/DLLUtils/RestAPI/RestApiDLL.cpp
    src/DLLUtils/RestAPI/RestApiDLL.h
    ../shared/src/GlobalDefinitions.h
    ../shared/src/GlobalDefinitions.cpp
    ../shared/src/FileData.cpp
    ../shared/src/FileData.h
    src/BackendTester/BackendTester.cpp
    src/BackendTester/BackendTester.h
    src/BackendTester/BackendTester.cpp
    src/gui/PasswordDialog.cpp
    src/gui/PasswordDialog.h
)

# Ensure the custom target is built before the main executable
add_dependencies(CppFileVaultMain compile_resources)

# Set target properties
set_target_properties(CppFileVaultMain PROPERTIES
    AUTOMOC ON
)

# Specify the path to the cpprest_2_10.lib file
target_link_directories(CppFileVaultMain PRIVATE "C:/Users/prang/.vcpkg-clion/vcpkg/packages/cpprestsdk_x64-windows/lib")

# Link the required libraries
target_link_libraries(CppFileVaultMain PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    SharedClasses
)

# Compilation options for warnings
if (MSVC)
  target_compile_options(CppFileVaultMain PRIVATE /W4 /GS)
else ()
  target_compile_options(CppFileVaultMain PRIVATE -Wall -Wextra)
endif ()

add_custom_command(TARGET CppFileVaultMain POST_BUILD
    # Erstelle die benötigten Verzeichnisse
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:CppFileVaultMain>/platforms

    # Kopiere die Qt-DLLs in das Verzeichnis der ausführbaren Datei
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Qt6::Core>
    $<TARGET_FILE:Qt6::Gui>
    $<TARGET_FILE:Qt6::Widgets>
    $<TARGET_FILE_DIR:CppFileVaultMain>

    # Kopiere qwindows.dll in das platforms-Verzeichnis
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${QT6_INSTALL_PREFIX}/plugins/platforms/$<IF:$<CONFIG:Debug>,qwindowsd.dll,qwindows.dll>"
    "$<TARGET_FILE_DIR:CppFileVaultMain>/platforms"
)
