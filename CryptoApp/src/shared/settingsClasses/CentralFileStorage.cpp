//
// Created by prang on 13.01.2025.
//

#include "CentralFileStorage.h"

#include <string>

bool CentralFileStorage::centralEncFileStorage = false;
bool CentralFileStorage::centralDecFileStorage = false;
unsigned long long CentralFileStorage::encFileCounter = 0;
unsigned long long CentralFileStorage::decFileCounter = 0;

fs::path CentralFileStorage::fileStoragePath = fs::current_path() / "centralStorage";

void CentralFileStorage::checkCentralFileStoragePath() {
  if (!fs::exists(fileStoragePath) || !fs::is_directory(fileStoragePath)
      || !fs::exists(fileStoragePath / "Encrypted") || !fs::is_directory(fileStoragePath / "Encrypted")
      || !fs::exists(fileStoragePath / "Decrypted") || !fs::is_directory(fileStoragePath / "Decrypted")
  ) {
    fs::create_directory(fileStoragePath);
    fs::create_directory(fileStoragePath / "Encrypted");
    fs::create_directory(fileStoragePath / "Decrypted");
  }
}

bool CentralFileStorage::moveFileToCentralStorage(const fs::path &filePath, bool isEncrypted) {
  checkCentralFileStoragePath();

  fs::path newFilePath;
  std::string newFileName;

  if (isEncrypted) {
    newFileName = "(" + std::to_string(++encFileCounter) + ") - " + filePath.filename().string();
    newFilePath = fileStoragePath / "Encrypted" / newFileName;
    fs::copy_file(filePath, newFilePath, fs::copy_options::overwrite_existing);
  } else {
    newFileName = "(" + std::to_string(++decFileCounter) + ") - " + filePath.filename().string();
    newFilePath = fileStoragePath / "Decrypted" / newFileName;
    fs::copy_file(filePath, newFilePath, fs::copy_options::overwrite_existing);
  }

  if (fs::exists(newFilePath)) {
    fs::remove(filePath);
  } else {
    return false;
  }

  return true;
}
