//
// Created by prang on 25.09.2024.
//

#include "SystemUtils.h"
#include <iostream>
#include <filesystem>
#include <windows.h>
#include <fstream>

namespace fs = std::filesystem;

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
