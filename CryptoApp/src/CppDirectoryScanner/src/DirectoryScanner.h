#ifndef FILEVAULTROOT_DIRECTORYSCANNER_H
#define FILEVAULTROOT_DIRECTORYSCANNER_H

#include <vector>
#include <filesystem>
#include <utility>
#include <algorithm>
#include <iostream>
#include <exception>
#include <cwctype>
#include <functional>
#include "../../shared/src/GlobalDefinitions.h"

#ifdef FILESCANNER_EXPORTS
#define FILESCANNER_API __declspec(dllexport)
#else
#define FILESCANNER_API __declspec(dllimport)
#endif

namespace fs = std::filesystem;

/**
 * Class for scanning directories and listing files.
 */
class FILESCANNER_API DirectoryScanner {
public:
  /**
   * Type definition for the callback function to be called for each found file.
   */
  using FileFoundCallback = std::function<void(const fs::path&)>;

  /**
   * Lists files in a directory, optionally searching only for decrypted files.
   *
   * @param directory The directory to scan for files.
   * @param searchOnlyForDecryptedFiles If true, only decrypted files will be listed.
   * @param fileList A vector to store the paths of the found files.
   * @param callback A callback function to be called for each found file.
   */
  void listFiles(const fs::path &directory, const bool searchOnlyForDecryptedFiles, std::vector<fs::path> &fileList, FileFoundCallback callback) const;

  /**
   * Checks if a file has an encrypted file extension.
   *
   * @param filepath The file path to check.
   * @return True if the file has an encrypted file extension, false otherwise.
   */
  static bool hasEncFileExtension(const fs::path &filepath);

private:
  /**
   * Checks if a path is valid by ensuring all characters are printable.
   *
   * @param p The path to check.
   * @return True if the path is valid, false otherwise.
   */
  static bool isValidPath(const fs::path &p);

  /**
   * Checks if a file path is system-critical.
   *
   * @param filePath The file path to check.
   * @return True if the file path is system-critical, false otherwise.
   */
  static bool isSystemCritical(const fs::path &filePath);

  /**
   * Flag to enable or disable debug printing.
   */
  const bool printDebug = true;
};

extern "C" {
  /**
   * Scans for files in a directory and lists them.
   *
   * @param originalFilePath The path to the directory to scan.
   * @param searchOnlyForDecryptedFiles If true, only decrypted files will be listed.
   * @param fileList A vector to store the paths of the found files.
   * @param callback A callback function to be called for each found file.
   */
  [[maybe_unused]] FILESCANNER_API void ScanForFilesInDirectory(const wchar_t *originalFilePath, const bool searchOnlyForDecryptedFiles, std::vector<fs::path> &fileList, DirectoryScanner::FileFoundCallback callback);
}

#endif //FILEVAULTROOT_DIRECTORYSCANNER_H