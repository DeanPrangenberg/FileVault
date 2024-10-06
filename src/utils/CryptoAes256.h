//
// Created by prang on 25.09.2024.
//

#ifndef RANSOMWARE_CRYPTOAES256_H
#define RANSOMWARE_CRYPTOAES256_H

#include "../../include/global.h"
#include <filesystem>

class CryptoAes256 {
public:

  void encryptSaveFile();

  void decryptSaveFile(const KeyIvAES& keyIv);

  bool testKeyOnEncryptedFileAES256(const fs::path& encryptedFilePath, const KeyIvAES& keyIv);

private:

  bool encryptFileAES256(const fs::path &inputFile, const fs::path &encryptedFile, const KeyIvAES& keyIv);

  bool decryptFileAES256(const fs::path &encryptedFile, const fs::path &decryptedFilePath, const KeyIvAES& keyIv);
};


#endif //RANSOMWARE_CRYPTOAES256_H
