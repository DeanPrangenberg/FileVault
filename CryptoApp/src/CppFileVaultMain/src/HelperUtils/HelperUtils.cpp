#include "HelperUtils.h"

/**
 * @brief Prints the FileID and EncryptionID data for debugging purposes.
 *
 * @param fileIDData The FileIDData structure containing the FileID and EncryptionID to debug.
 */
void debugFileIDData(const HelperUtils::FileIDData &fileIDData) {
  std::cout << "***************************************************" << std::endl;
  std::cout << "FileID: ";
  for (const auto &byte: *fileIDData.FileID) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }
  std::cout << std::endl;

  std::cout << "EncryptionID: ";
  for (const auto &byte: *fileIDData.EncryptionID) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }
  std::cout << std::dec << std::endl;

  std::cout << "newEncryptedFilePath :" << fileIDData.newEncryptedFilePath << std::endl;
  std::cout << "***************************************************" << std::endl;
}

/**
 * @brief Repairs lost encrypted file structures in the specified directories.
 *
 * @param directorys A vector of paths representing the directories to scan and repair.
 */
void HelperUtils::repairLostEncFileStructs(std::vector<fs::path> &directorys) {
  CryptoDLL cryptoDll;

  // update id
  std::vector<unsigned char> updateIdVec(64);
  cryptoDll.getCurrentTimeHash(updateIdVec.data());

  auto totalFiles = scanForAllFiles(directorys);
  auto lostFiles = checkFilesForFileID(totalFiles);
  auto fileDataList = findFileDataStructsInDatabase(lostFiles);
  updateFileDataInDB(fileDataList, lostFiles, updateIdVec);
}

/**
 * @brief Scans the specified directories for all files.
 *
 * @param directorys A vector of paths representing the directories to scan.
 * @return A vector of paths representing all found files.
 */
std::vector<fs::path> HelperUtils::scanForAllFiles(const std::vector<fs::path> &directorys) {
  FileScannerDLL fileScannerDll;
  std::vector<fs::path> totalFiles;

  if (printDebug) std::cout << "repairLostEncFileStructs: Scanning for all files" << std::endl;
  for (const auto &directory: directorys) {
    std::vector<fs::path> foundFiles;
    fileScannerDll.ScanDirectory(directory, true, foundFiles, [](const fs::path){});
    totalFiles.insert(totalFiles.end(), foundFiles.begin(), foundFiles.end());
  }
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Found " << totalFiles.size() << " encrypted files" << std::endl;

  return totalFiles;
}

/**
 * @brief Checks the specified files for FileID and EncryptionID.
 *
 * @param totalFiles A vector of paths representing the files to check.
 * @return A vector of FileIDData structures representing the files with found FileID and EncryptionID.
 */
std::vector<HelperUtils::FileIDData> HelperUtils::checkFilesForFileID(const std::vector<fs::path> &totalFiles) {
  FileMarkDLL fileMarkDll;
  std::vector<FileIDData> lostFiles;

  if (printDebug) std::cout << "repairLostEncFileStructs: Checking files for FileID" << std::endl;
  for (const auto &filePath: totalFiles) {
    auto FileID = std::make_shared<std::vector<unsigned char>>(64);
    auto EncryptionID = std::make_shared<std::vector<unsigned char>>(64);
    if (fileMarkDll.extractIDsFromFile(&filePath, FileID.get(), EncryptionID.get())) {
      if (printDebug) std::cout << "FileID and EncryptionID found for file: " << filePath << std::endl;
      lostFiles.push_back({FileID, EncryptionID, filePath});
    } else {
      std::cerr << "Failed to extract FileID and EncryptionID for file: " << filePath << std::endl;
    }
  }
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Found " << lostFiles.size() << " lost files with FileID and EncryptionID" << std::endl;

  return lostFiles;
}

/**
 * @brief Finds the FileData structures in the database for the specified lost files.
 *
 * @param lostFiles A vector of FileIDData structures representing the lost files.
 * @return A vector of FileData structures found in the database.
 */
std::vector<FileData> HelperUtils::findFileDataStructsInDatabase(const std::vector<FileIDData> &lostFiles) {
  RestApiDLL restApiDll;
  std::vector<FileData> fileDataList;

  if (printDebug) std::cout << "repairLostEncFileStructs: Finding FileData structs in database" << std::endl;
  for (const auto &FileIDDataStruct: lostFiles) {
    debugFileIDData(FileIDDataStruct);
    FileData fileData{};
    fileData.FileID->assign(FileIDDataStruct.FileID->begin(), FileIDDataStruct.FileID->end());
    fileData.EncryptionID->assign(FileIDDataStruct.EncryptionID->begin(), FileIDDataStruct.EncryptionID->end());

    if (fileData.FileID->empty() || fileData.EncryptionID->empty()) {
      std::cerr << "repairLostEncFileStructs: FileID or EncryptionID is null" << std::endl;
      continue;
    } else {
      if (printDebug) {
        std::cout << "repairLostEncFileStructs: FileID: " << fileData.toHexString(fileData.FileID) << std::endl;
        std::cout << "repairLostEncFileStructs: EncryptionID: " << fileData.toHexString(fileData.EncryptionID) << std::endl;
      }
    }

    if (restApiDll.SearchEntry(fileData)) {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: Found struct for file: " << FileIDDataStruct.newEncryptedFilePath << std::endl;
        fileDataList.push_back(fileData);
      if (printDebug) {
        std::cout << "repairLostEncFileStructs: FileID: " << fileData.toHexString(fileData.FileID) << std::endl;
        std::cout << "repairLostEncFileStructs: EncryptionID: " << fileData.toHexString(fileData.EncryptionID) << std::endl;
      }
    } else {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: Could not find struct for file: " << FileIDDataStruct.newEncryptedFilePath << std::endl;
    }
  }
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Found " << fileDataList.size() << " FileData structs in the db" << std::endl;

  return fileDataList;
}

/**
 * @brief Updates the FileData structures in the database with the new encrypted file path.
 *
 * @param fileDataList A vector of FileData structures to update.
 * @param lostFiles A vector of FileIDData structures representing the lost files.
 * @param updateId The update ID to set in the FileData structures.
 */
void HelperUtils::updateFileDataInDB(std::vector<FileData> &fileDataList, const std::vector<FileIDData> &lostFiles, std::vector<unsigned char> &updateId) {
  RestApiDLL restApiDll;

  if (printDebug)
    std::cout << "repairLostEncFileStructs: Updating EncryptedFilePath in the FileData structs" << std::endl;
  for (auto &fileData: fileDataList) {
    if (fileData.FileID->empty() || fileData.EncryptionID->empty()) {
      if (printDebug) std::cout << "repairLostEncFileStructs: FileID or EncryptionID is null" << std::endl;
      continue;
    }
    bool updated = false;
    for (const auto &lostFile: lostFiles) {
      if (printDebug) std::cout << "repairLostEncFileStructs: Comparing FileIDs and EncryptionIDs" << std::endl;
      if (printDebug) {
        std::cout << "repairLostEncFileStructs: LostFileID: " << fileData.toHexString(lostFile.FileID) << std::endl;
        std::cout << "repairLostEncFileStructs: FileID: " << fileData.toHexString(fileData.FileID) << std::endl;
        std::cout << "repairLostEncFileStructs: LostEncryptionID: " << fileData.toHexString(lostFile.EncryptionID) << std::endl;
        std::cout << "repairLostEncFileStructs: EncryptionID: " << fileData.toHexString(fileData.EncryptionID) << std::endl;
      }

      bool equal = true;
      for (size_t i = 0; i < lostFile.FileID->size(); i++) {
        if (std::memcmp(lostFile.FileID->data(), fileData.FileID->data(), lostFile.FileID->size()) != 0
            || std::memcmp(lostFile.EncryptionID->data(), fileData.EncryptionID->data(), lostFile.EncryptionID->size()) != 0) {
          equal = false;
          if (printDebug) std::cout << "repairLostEncFileStructs: FileIDs or EncryptionIDs do not match" << std::endl;
          break;
        }
      }

      if (equal) {
        if (printDebug) std::cout << "repairLostEncFileStructs: FileIDs and EncryptionIDs match" << std::endl;

        if (printDebug) std::cout << "repairLostEncFileStructs: Updating EncryptedFilePath" << std::endl;
        fileData.EncryptedFilePath->assign(lostFile.newEncryptedFilePath);
        fileData.LastUpdateID->assign(updateId.begin(), updateId.end());

        if (printDebug) std::cout << "repairLostEncFileStructs: Updated EncryptedFilePath" << std::endl;
        // Replace the old FileData struct in the database with the updated one
        if (restApiDll.ReplaceEntry(fileData)) {
          std::wcout << L"repairLostEncFileStructs: Replaced struct for file: " << fileData.EncryptedFilePath->wstring() << std::endl;
          updated = true;
        } else {
          std::wcout << L"repairLostEncFileStructs: Could not replace struct for file: " << fileData.EncryptedFilePath->wstring() << std::endl;
        }

        break;
      } else {
        if (printDebug) std::cout << "repairLostEncFileStructs: FileID and LostFileID or EncryptionID and LostEncryptionID do not match" << std::endl;
      }
    }
    if (!updated) {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: Could not find a matching FileID or EncryptionID for struct update ##" << std::endl;
      auto it = std::remove(fileDataList.begin(), fileDataList.end(), fileData);
      if (it != fileDataList.end()) {
        fileDataList.erase(it, fileDataList.end());
        if (printDebug) std::cout << "repairLostEncFileStructs: Removed struct from list" << std::endl;
      }
    }
  }

  if (printDebug)
    std::cout << "repairLostEncFileStructs: Updated " << fileDataList.size() << " FileData structs in the db" << std::endl;
}

/**
 * @brief Repairs all lost encrypted file structures on all drives.
 */
void HelperUtils::repairAllLostStruct() {
  auto drives = SystemUtils::getAllDrives();
  repairLostEncFileStructs(drives);
}

/**
 * @brief Decrypts the specified files.
 *
 * @param filePaths A vector of paths representing the files to decrypt.
 * @return An unordered map with the decryption results.
 */
std::unordered_map<std::string, int> HelperUtils::decryptFiles(const std::vector<fs::path> &filePaths) {
  CryptoDLL cryptoDll;
  RestApiDLL restApiDll;
  FileMarkDLL fileMarkDll;
  std::unordered_map<std::string, int> results;
  std::vector<FileData> fileDataVec;

  for (const auto &filePath : filePaths) {
    // Create the FileData structure
    FileData fileData;
    fileData.EncryptionID->resize(64);
    fileData.FileID->resize(64);

    fileMarkDll.extractIDsFromFile(&filePath, fileData.FileID.get(), fileData.EncryptionID.get());

    if (!restApiDll.SearchEntry(fileData)) {
      results.insert({"Search", 1}); // Error finding the entry
      continue;
    }
    fileData.EncryptedFilePath->assign(filePath);
    fileDataVec.push_back(fileData);
  }

  // Decrypt the files
  std::vector<bool> decryptResults(fileDataVec.size(), false);
  cryptoDll.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), decryptResults);

  for (size_t i = 0; i < fileDataVec.size(); ++i) {
    std::string algorithm(fileDataVec[i].AlgorithmenType->begin(), fileDataVec[i].AlgorithmenType->end());

    // Check if decryption was successful
    if (!decryptResults[i]) {
      results.insert({algorithm, 2}); // Error during decryption
      continue;
    }

    // Delete the entry from the database
    if (!restApiDll.DeleteEntry(fileDataVec[i])) {
      results.insert({algorithm, 3}); // Error deleting the entry
      continue;
    }

    // Clean up the FileData structure
    fileDataVec[i].Key->clear();
    fileDataVec[i].Iv->clear();
    results.insert({algorithm, -1}); // Success
  }

  return results;
}

/**
 * @brief Encrypts the specified files using the specified algorithms.
 *
 * @param filePaths A vector of paths representing the files to encrypt.
 * @param algorithm A vector of strings representing the algorithms to use for encryption.
 * @return A vector of integers representing the encryption results.
 */
std::vector<int> HelperUtils::encryptFiles(const std::vector<fs::path> &filePaths, const std::vector<std::string> &algorithm) {
  CryptoDLL cryptoDll;
  RestApiDLL restApiDll;
  AttributeUtils structUtils;
  std::vector<int> results;
  std::vector<FileData> fileDataVec;

  for (int i = 0; i < filePaths.size(); i++) {
    // Create the FileData structure
    FileData fileData;
    if (algorithm[i] == "AES-128") {
      structUtils.createFileDataStruct(globalDefinitions::AlgorithmType::AES128, filePaths[i], fileData);
    } else {
      structUtils.createFileDataStruct(globalDefinitions::AlgorithmType::AES256, filePaths[i], fileData);
    }
    fileDataVec.push_back(fileData);
  }

  // Encrypt the file
  std::vector<bool> encryptResults(fileDataVec.size(), false);
  cryptoDll.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), encryptResults);

  for (auto fileData : fileDataVec) {
    // Check if encryption was successful
    if (fileData.EncryptedFilePath->empty() || !encryptResults[0]) {
      results.push_back(1); // Error during encryption
      continue;
    }

    // Insert the entry into the database
    if (!restApiDll.InsertEntry(fileData)) {
      results.push_back(2); // Error inserting the entry
      continue;
    }

    // Clean up the FileData structure
    fileData.Key->clear();
    fileData.Iv->clear();
    results.push_back(-1); // Success
  }

  return results;
}

/**
 * @brief Checks the state of the files in the database.
 *
 * @return A vector of integers representing the number of valid and invalid files.
 */
std::vector<int> HelperUtils::checkDBFileState() {
  RestApiDLL restApiDll;
  const auto& fileDataList = restApiDll.GetAllFileIDsAndEncryptedPaths();
  std::vector<int> results = {0, 0, 0};  // [OK, Mismatch, Extraction Errors]

  FileMarkDLL fileMarkDll;
  for (const auto& fileData : fileDataList) {
    std::vector<unsigned char> fileID;
    fileID.reserve(128);
    std::vector<unsigned char> encryptionID;
    encryptionID.reserve(128);

    fileData.EncryptedFilePath->assign(fileData.EncryptedFilePath->lexically_normal());

    if (!fileMarkDll.extractIDsFromFile(fileData.EncryptedFilePath.get(), &fileID, &encryptionID)) {
      results[2]++;
      Logs::writeToErrorLog("Extraktion fehlgeschlagen für: " + fileData.EncryptedFilePath->string());
      continue;
    }

    fileID.resize(64);
    encryptionID.resize(64);

    fileData.FileID->erase(std::remove(fileData.FileID->begin(),
                                       fileData.FileID->end(), '\n'), fileData.FileID->end());
    fileData.EncryptionID->erase(std::remove(fileData.EncryptionID->begin(),
                                       fileData.EncryptionID->end(), '\n'), fileData.EncryptionID->end());

    const bool isMatch = (fileID == *fileData.FileID) && (encryptionID == *fileData.EncryptionID);

    if (isMatch) {
      results[0]++;
    } else {
      results[1]++;
      Logs::writeToErrorLog("ID-Mismatch in: " + fileData.EncryptedFilePath->string());
      Logs::writeToErrorLog("DB FileID: " + FileData::toHexString(fileData.FileID));
      Logs::writeToErrorLog("File FileID: " + FileData::toHexString(std::make_shared<std::vector<unsigned char>>(fileID)));
      Logs::writeToErrorLog("DB EncryptionID: " + FileData::toHexString(fileData.EncryptionID));
      Logs::writeToErrorLog("File EncryptionID: " + FileData::toHexString(std::make_shared<std::vector<unsigned char>>(encryptionID)));
    }
  }
  return results;
}

/**
 * @brief Converts a filesystem path to a wide character string.
 *
 * @param filePath The filesystem path to convert.
 * @return A pointer to the wide character string representing the path.
 */
wchar_t* HelperUtils::convertPathToWchar(const fs::path &filePath) {
  std::wstring filePathWStr = filePath.wstring();
  auto* filePathWChar = new wchar_t[filePathWStr.size() + 1];
  std::copy(filePathWStr.begin(), filePathWStr.end(), filePathWChar);
  filePathWChar[filePathWStr.size()] = L'\0';
  return filePathWChar;
}

/**
 * @brief Saves the database to a file in the specified directory.
 *
 * @param targetDir The directory where the database file will be saved.
 * @param database An unordered map representing the database to save.
 */
void HelperUtils::saveDatabaseToFile(const fs::path &targetDir, const std::unordered_map<std::string, std::string> &database) {
  auto timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream ss;
  ss << "expoDatabase" << timestamp << ".FvDB";
  fs::path targetPath = targetDir / ss.str();

  std::ofstream outFile(targetPath, std::ios::binary);
  if (!outFile) {
    throw std::runtime_error("Failed to open file for writing: " + targetPath.string());
  }

  size_t mapSize = database.size();
  outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

  for (const auto& pair : database) {
    size_t keySize = pair.first.size();
    size_t valueSize = pair.second.size();

    outFile.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
    outFile.write(pair.first.data(), keySize);

    outFile.write(reinterpret_cast<const char*>(&valueSize), sizeof(valueSize));
    outFile.write(pair.second.data(), valueSize);
  }

  outFile.close();
}

/**
 * @brief Loads the database from a file in the specified directory.
 *
 * @param targetPath The path to the database file.
 * @return An unordered map representing the loaded database.
 */
std::unordered_map<std::string, std::string> HelperUtils::loadDatabaseFromFile(const fs::path &targetPath) {
  std::ifstream inFile(targetPath, std::ios::binary);
  if (!inFile) {
    throw std::runtime_error("Failed to open file for reading: " + targetPath.string());
  }

  std::unordered_map<std::string, std::string> database;
  size_t mapSize;
  inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

  for (size_t i = 0; i < mapSize; ++i) {
    size_t keySize, valueSize;
    inFile.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));

    std::string key(keySize, '\0');
    inFile.read(&key[0], keySize);

    inFile.read(reinterpret_cast<char*>(&valueSize), sizeof(valueSize));

    std::string value(valueSize, '\0');
    inFile.read(&value[0], valueSize);

    database[key] = value;
  }

  inFile.close();
  return database;
}