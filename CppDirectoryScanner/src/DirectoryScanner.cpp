//
// Created by prang on 21.10.2024.
//

#include "DirectoryScanner.h"
#include "../../CppFileVaultMain/src/GlobalDefinitions.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <exception>
#include <cwctype>


namespace fs = std::filesystem;

std::vector<fs::path> DirectoryScanner::listFiles(const fs::path &directory, const bool &encryptionMode) {
  std::vector<fs::path> fileList;
  if (!fs::exists(directory) || !fs::is_directory(directory)) {
    std::wcerr << "Error: Invalid directory: " << directory << std::endl;
    return fileList;
  }

  std::vector<fs::path> dirs;
  dirs.push_back(directory);

  while (!dirs.empty()) {
    if (printDebug) std::wcout << L"Dirs size: " << dirs.size() << std::endl;
    if (printDebug) std::wcout << L"Found " << fileList.size() << " files" << std::endl;
    fs::path current_dir = dirs.back();
    dirs.pop_back();

    if (isSystemCritical(current_dir)) {
      std::wcerr << L"Skipping system-critical directory: " << current_dir << std::endl;
      continue;
    }

    if (printDebug) std::wcout << L"Start Scanning directory: " << current_dir << std::endl;
    for (const auto &entry: fs::directory_iterator(current_dir)) {
      if (printDebug) std::wcout << L"Entry: " << entry.path() << std::endl;
      try {
        if (!isValidPath(entry.path()) ||
            (fs::status(entry).permissions() & fs::perms::owner_read) == fs::perms::none) {
          continue;
        }

        if (fs::is_directory(entry) && !isSystemCritical(entry.path())) {
          dirs.push_back(entry.path());
        } else if (fs::is_regular_file(entry)
                   && !isSystemCritical(entry.path())
                   && fs::file_size(entry.path()) > 0) {
          if (!encryptionMode || hasEncFileExtension(entry.path())) {
            if (printDebug) std::wcout << L"Found file: " << entry.path() << std::endl;
            fileList.push_back(entry.path());
          }
        }
      } catch (const std::exception &e) {
        std::wcerr << L"Error accessing entry: " << entry.path() << L" -> " << e.what() << std::endl;
      }
    }
  }

  std::sort(fileList.begin(), fileList.end(), [](const fs::path &a, const fs::path &b) {
    return fs::file_size(a) > fs::file_size(b);
  });

  if (printDebug) std::wcout << L"Found " << fileList.size() << L" files in directory: " << directory << std::endl;
  return fileList;
}

bool DirectoryScanner::isValidPath(const fs::path &p) {
  // Check if each character in the path is printable
  for (const auto &ch: p.wstring()) {
    if (!std::iswprint(ch)) {
      std::wcerr << L"Invalid character in path: " << p << std::endl;
      return false;
    }
  }
  return true;
}

bool DirectoryScanner::isSystemCritical(const fs::path &filePath) {
  // List of critical paths
  const std::vector<std::wstring> critical_paths = {
      L"Windows",
      L"Program Files",
      L"Program Files (x86)",
      L"System Volume Information",
      L"$Recycle.Bin",
      L"$RECYCLE.BIN",
      L"PerfLogs",
      L"Boot"
  };

  // Check if the file path contains any critical path
  for (const auto &critical_path : critical_paths) {
    if (filePath.wstring().find(critical_path) != std::wstring::npos) {
      return true; // Path is critical
    }
  }

  return false; // Path is not critical
}

bool DirectoryScanner::hasEncFileExtension(const fs::path &filepath) {
  fs::path personalDataExtensions = globalDefinitions::encFileSuffix;

  // Get the extension of the file path
  fs::path extension = filepath.extension();

  return extension == personalDataExtensions;
}

const wchar_t **DirectoryScanner::convertVectorToWCharArray(const std::vector<std::filesystem::path> &paths) {
  const wchar_t **result = new const wchar_t *[paths.size() + 1]; // +1 for null terminator
  for (size_t i = 0; i < paths.size(); ++i) {
    result[i] = _wcsdup(paths[i].c_str()); // Duplicate the string to ensure it is properly managed
  }
  result[paths.size()] = nullptr; // Null-terminate the array
  return result;
}

extern "C" {
  const wchar_t **ScanForFilesInDirectory(const wchar_t *originalFilePath, const bool *encryptionMode) {
    fs::path directory(originalFilePath);
    DirectoryScanner directoryScanner;
    return directoryScanner.convertVectorToWCharArray(directoryScanner.listFiles(directory, encryptionMode));
  }
}