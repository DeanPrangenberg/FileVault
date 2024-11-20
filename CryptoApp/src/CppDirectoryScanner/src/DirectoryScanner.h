//
// Created by prang on 21.10.2024.
//

#ifndef FILEVAULTROOT_DIRECTORYSCANNER_H
#define FILEVAULTROOT_DIRECTORYSCANNER_H

#include <vector>
#include <filesystem>

#ifdef FILESCANNER_EXPORTS
#define FILESCANNER_API __declspec(dllexport)
#else
#define FILESCANNER_API __declspec(dllimport)
#endif

namespace fs = std::filesystem;

class FILESCANNER_API DirectoryScanner {
public:
  std::vector<fs::path> listFiles(const fs::path &directory, const bool searchOnlyForDecryptedFiles) const;

  static const wchar_t **convertVectorToWCharArray(const std::vector<fs::path> &paths);

private:
  static bool isValidPath(const fs::path &p);

  static bool isSystemCritical(const fs::path &filePath);

  static bool hasEncFileExtension(const fs::path &filepath);

  const bool printDebug = false;
};

extern "C" {
[[maybe_unused]] FILESCANNER_API const wchar_t **ScanForFilesInDirectory(const wchar_t *originalFilePath, const bool searchOnlyForDecryptedFiles);
}

#endif //FILEVAULTROOT_DIRECTORYSCANNER_H
