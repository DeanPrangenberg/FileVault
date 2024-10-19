//
// Created by prang on 25.09.2024.
//

#include "FileUtils.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <exception>
#include <stack>

namespace fs = std::filesystem;

std::vector<fs::path> FileUtils::scanPathForFiles(const std::vector<fs::path>& directoryList, const bool encryptionMode) {
  std::vector<fs::path> file_list;

  for (const auto& drive : directoryList) {
    try {
      // Check if the drive/path exists before iterating
      if (!fs::exists(drive)) {
        std::cerr << "Error: Directory does not exist: " << drive << std::endl;
        continue;
      }

      // Check if the path is a directory
      if (!fs::is_directory(drive)) {
        std::cerr << "Error: Path is not a directory: " << drive << std::endl;
        continue;
      }

      std::stack<fs::path> dirs; // Stack for directories
      dirs.push(drive); // Start with the initial drive

      while (!dirs.empty()) {
        fs::path current_dir = dirs.top(); // Get the current directory
        dirs.pop(); // Remove it from the stack

        try {
          // Check if the directory is accessible and not hidden
          if ((fs::status(current_dir).permissions() & fs::perms::owner_read) == fs::perms::none ||
              (fs::is_directory(current_dir) && fs::status(current_dir).permissions() == fs::perms::none)) {
            std::cerr << "Skipping inaccessible or hidden directory: " << current_dir << std::endl;
            continue;
          }

          // Iterate over the directory entries
          for (const auto& entry : fs::directory_iterator(current_dir)) {
            try {
              // Skip hidden directories and files
              if ((fs::status(entry).permissions() & fs::perms::owner_read) == fs::perms::none) {
                std::cerr << "Skipping hidden or inaccessible entry: " << entry.path() << std::endl;
                continue;
              }

              // Check if the path is valid
              if (!isValidPath(entry.path())) {
                continue; // Skip invalid paths
              }

              if (fs::is_directory(entry)) {
                dirs.push(entry.path()); // Push subdirectories onto the stack
              } else if (fs::is_regular_file(entry) && !isSystemCritical(entry.path())) {
                // Check if the file has content (is not empty)
                if (fs::file_size(entry.path()) > 0) {
                  file_list.push_back(entry.path()); // Only add the file if it has content
                } else {
                  std::cerr << "Skipping empty file: " << entry.path() << std::endl;
                }
              }
            } catch (const fs::filesystem_error& e) {
              // Log the error and continue
              std::cerr << "Error accessing file: " << entry.path() << " -> " << e.what() << " (File system error)"
                        << std::endl;
              continue;
            } catch (const std::exception& e) {
              // Log the error and continue
              std::cerr << "Error accessing file: " << entry.path() << " -> " << e.what() << " (General error)"
                        << std::endl;
              continue;
            }
          }
        } catch (const fs::filesystem_error& e) {
          // Log the directory error and continue scanning
          std::cerr << "Filesystem error accessing directory: " << current_dir << " -> " << e.what() << std::endl;
          continue;
        } catch (const std::exception& e) {
          // Log the general error and continue scanning
          std::cerr << "General error accessing directory: " << current_dir << " -> " << e.what() << std::endl;
          continue;
        }
      }
    } catch (const fs::filesystem_error& e) {
      // Log the drive-level error and continue
      std::cerr << "Filesystem error accessing drive: " << drive << " -> " << e.what() << std::endl;
      continue;
    } catch (const std::exception& e) {
      // Log the general drive error and continue
      std::cerr << "General error accessing drive: " << drive << " -> " << e.what() << std::endl;
      continue;
    }
  }

  return file_list;
}

bool FileUtils::isValidPath(const fs::path& p) {
  // Check if each character in the path is printable
  for (const auto& ch : p.string()) {
    if (!std::iswprint(ch)) {
      std::cerr << "Invalid character in path: " << p << std::endl;
      return false;
    }
  }
  return true;
}

bool FileUtils::isSystemCritical(const fs::path& file_path) {
  // Hole den Laufwerksbuchstaben des Pfades
  std::wstring driveLetter = file_path.root_name().wstring();

  // Liste der kritischen Pfade
  const std::vector<fs::path> critical_paths = {
      L"Windows",
      L"Program Files",
      L"Program Files (x86)",
      L"System Volume Information",
      L"$Recycle.Bin",
      L"PerfLogs",
      L"Boot"
  };

  // Vollständigen kritischen Pfad bilden und vergleichen
  for (const auto& critical_path : critical_paths) {
    fs::path fullCriticalPath = fs::path(driveLetter) / critical_path;

    // Überprüfen, ob der Dateipfad den kritischen Pfad enthält
    if (file_path.string().find(fullCriticalPath.string()) == 0) {
      return true; // Pfad ist kritisch
    }
  }

  return false; // Pfad ist nicht kritisch
}
