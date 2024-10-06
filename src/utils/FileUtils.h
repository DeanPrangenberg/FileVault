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
  std::vector<fs::path> listFiles(const std::vector<fs::path> &directoryList, bool encryptionMode);

  std::vector<CryptoData> loadCryptoData();

  void saveCryptoData(const CryptoData &data);

private:
  static bool isValidPath(const fs::path& p);

  static bool isSystemCritical(const fs::path &file_path);

  static bool hasValidExtension(const fs::path &filepath, bool encryptionMode);
};


#endif // FILE_UTILS_H
