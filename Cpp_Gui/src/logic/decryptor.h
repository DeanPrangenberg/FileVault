//
// Created by prang on 25.09.2024.
//

#ifndef RANSOMWARE_DECRYPTOR_H

#include <filesystem>

namespace fs = std::filesystem;

class Decrypt {
public:
  void startDecryptionProcess();
private:
  void printPathList(std::vector<fs::path> &file_list);
};
#define RANSOMWARE_DECRYPTOR_H

#endif //RANSOMWARE_DECRYPTOR_H
