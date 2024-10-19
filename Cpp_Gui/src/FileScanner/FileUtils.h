//
// Created by prang on 25.09.2024.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include "../../include/global.h"
#include <fstream>

namespace fs = std::filesystem;

class FileUtils {
public:
  static std::vector<fs::path> scanPathForFiles(const std::vector<fs::path> &directoryList, const bool encryptionMode);
private:
  static bool isValidPath(const fs::path& p);
  static bool isSystemCritical(const fs::path &file_path);
};


#endif // FILE_UTILS_H
