//
// Created by prang on 13.01.2025.
//

#ifndef CENTRALSTORAGE_H
#define CENTRALSTORAGE_H

#include <filesystem>

namespace fs = std::filesystem;

class CentralFileStorage {
public:
  CentralFileStorage() = default;
  static bool centralEncFileStorage;
  static bool centralDecFileStorage;
  static fs::path fileStoragePath;
  static bool moveFileToCentralStorage(const fs::path &filePath, bool isEncrypted);
private:
  static void checkCentralFileStoragePath();
  static unsigned long long encFileCounter;
  static unsigned long long decFileCounter;
};



#endif //CENTRALSTORAGE_H
