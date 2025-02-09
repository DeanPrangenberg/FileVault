//
// Created by prang on 25.09.2024.
//

#include "SystemUtils.h"
#include "src/GlobalDefinitions.h"

namespace fs = std::filesystem;

/**
 * @brief Retrieves the Windows folder path.
 *
 * This function retrieves the path to the Windows folder on the system.
 * It uses the GetWindowsDirectoryW function to get the path and then processes
 * it to extract the drive letter.
 *
 * @return The path to the Windows folder.
 */
fs::path SystemUtils::getWindowsFolder() {
  WCHAR buffer[MAX_PATH];
  if (GetWindowsDirectoryW(buffer, MAX_PATH) == 0) {
    return fs::path();
  }

  fs::path windowsDrive(buffer);

  std::wstring windowsDriveString = windowsDrive.wstring();
  size_t pos = windowsDriveString.find(L"\\Windows");

  if (pos != std::wstring::npos) {
    windowsDrive = windowsDriveString.substr(0, pos);
  }

  return windowsDrive / fs::path();
}

/**
 * @brief Retrieves all logical drives on the system.
 *
 * This function retrieves all logical drives available on the system.
 * It uses the GetLogicalDriveStringsW function to get the drive strings
 * and then processes them to create a vector of paths representing each drive.
 *
 * @return A vector of paths representing all logical drives.
 */
std::vector<fs::path> SystemUtils::getAllDrives() {
  std::vector<fs::path> drives;
  WCHAR buffer[MAX_PATH];
  DWORD size = GetLogicalDriveStringsW(MAX_PATH, buffer);

  if (size == 0) {
    return drives;
  }

  WCHAR *drive = buffer;
  while (*drive) {
    drives.push_back(fs::path(drive));
    drive += wcslen(drive) + 1;
  }

  return drives;
}