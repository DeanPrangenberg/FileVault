//
// Created by prang on 25.09.2024.
//

#ifndef RANSOMWARE_ENCRYPTOR_H

#include <filesystem>

namespace fs = std::filesystem;

class Encrypt {
public:
  void startEncryptionProcess();
private:
  static void createSaveFile();

  void printPathList(std::vector<fs::path> &file_list);
};

#define RANSOMWARE_ENCRYPTOR_H

#endif //RANSOMWARE_ENCRYPTOR_H
