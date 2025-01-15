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

class FILESCANNER_API DirectoryScanner {
public:
  using FileFoundCallback = std::function<void(const fs::path&)>;

  void listFiles(const fs::path &directory, const bool searchOnlyForDecryptedFiles, std::vector<fs::path> &fileList, FileFoundCallback callback) const;

  static bool hasEncFileExtension(const fs::path &filepath);

private:
  static bool isValidPath(const fs::path &p);
  static bool isSystemCritical(const fs::path &filePath);

  const bool printDebug = false;
};

extern "C" {
[[maybe_unused]] FILESCANNER_API void ScanForFilesInDirectory(const wchar_t *originalFilePath, const bool searchOnlyForDecryptedFiles, std::vector<fs::path> &fileList, DirectoryScanner::FileFoundCallback callback);
}

#endif //FILEVAULTROOT_DIRECTORYSCANNER_H