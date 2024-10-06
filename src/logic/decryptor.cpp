//
// Created by prang on 25.09.2024.
//

#include <vector>
#include <iostream>
#include "decryptor.h"
#include "../utils/FileUtils.h"
#include "../utils/CryptoAes128.h"


void Decrypt::printPathList(std::vector<fs::path> &file_list) {
  int pathAmount = 0;
  for (const auto &path: file_list) {
    pathAmount++;
    std::cout << "Path (" << pathAmount << "): " << path << std::endl;
  }
}

void Decrypt::startDecryptionProcess() {
  FileUtils fileUtils;
  CryptoAes128 cryptoAes128;

  std::vector<CryptoData> cryptoDataVector = fileUtils.loadCryptoData();

  size_t pathAmount = cryptoDataVector.size();

  // Entschlüsseln und anschließend löschen der verschlüsselten Dateien
  int decryptionCounter = 0;

  for (auto &cryptoData: cryptoDataVector) {
    decryptionCounter++;
    const fs::path file = cryptoData.encryptedPath;
    fs::path decryptedFilePath = file.parent_path() / file.stem();

    if (cryptoAes128.decryptFileAES128(cryptoData, decryptedFilePath)) {
      std::cout << "Decryption Succeeded at Path (" << decryptionCounter << "/" << pathAmount <<  "): " << decryptedFilePath << std::endl;
    } else {
      std::cerr << "Decrypting failed at Path (" << decryptionCounter << "/" << pathAmount <<  "): " << file << std::endl;
      continue;
    }

    if (fs::exists(decryptedFilePath)) {
      fs::remove(file);
    } else {
      std::cerr << "Decrypted file not found for deletion: Path (" << decryptionCounter << "/" << pathAmount <<  "): " << decryptedFilePath << std::endl;
    }
  }

  std::cout << decryptionCounter << "/" << pathAmount << " files have been decrypted." << std::endl;

  if (decryptionCounter == cryptoDataVector.size() && cryptoDataVector.size() > 0) {
    fs::remove(KEY_IV_SAVE_FILE);
    std::cout << "Save file got deleted!" << std::endl;
  }
}
