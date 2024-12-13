#include <vector>
#include <array>
#include <iostream>
#include "HelperUtils.h"
#include "../System/SystemUtils.h"
#include "../DLLUtils/FileScannerDLL.h"
#include "../DLLUtils/RestApiDLL.h"
#include "../DLLUtils/FileMarkDLL.h"
#include "../DLLUtils/CryptoDLL.h"

void debugFileIDData(const HelperUtils::FileIDData &fileIDData) {
  std::cout << "***************************************************" << std::endl;
  std::cout << "FileID: ";
  for (const auto &byte: fileIDData.FileID) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }
  std::cout << std::endl;

  std::cout << "EncryptionID: ";
  for (const auto &byte: fileIDData.EncryptionID) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }
  std::cout << std::dec << std::endl;

  std::cout << "newEncryptedFilePath :" << fileIDData.newEncryptedFilePath << std::endl;
  std::cout << "***************************************************" << std::endl;
}

void HelperUtils::repairLostEncFileStructs(std::vector<fs::path> &directorys) {
  CryptoDLL cryptoDll;

  // update id
  auto updateId = new unsigned char[64];
  cryptoDll.getCurrentTimeHash(updateId);

  auto totalFiles = scanForAllFiles(directorys);
  auto lostFiles = checkFilesForFileID(totalFiles);
  auto fileDataList = findFileDataStructsInDatabase(lostFiles);
  updateFileDataInDB(fileDataList, lostFiles, updateId);
}

std::vector<fs::path> HelperUtils::scanForAllFiles(const std::vector<fs::path> &directorys) {
  FileScannerDLL fileScannerDll;
  std::vector<fs::path> totalFiles;

  if (printDebug) std::cout << "repairLostEncFileStructs: Scanning for all files" << std::endl;
  for (const auto &directory: directorys) {
    auto foundFiles = fileScannerDll.ScanDirectory(directory, true);
    totalFiles.insert(totalFiles.end(), foundFiles.begin(), foundFiles.end());
  }
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Found " << totalFiles.size() << " encrypted files" << std::endl;

  return totalFiles;
}

std::vector<HelperUtils::FileIDData> HelperUtils::checkFilesForFileID(const std::vector<fs::path> &totalFiles) {
  FileMarkDLL fileMarkDll;
  std::vector<FileIDData> lostFiles;

  if (printDebug) std::cout << "repairLostEncFileStructs: Checking files for FileID" << std::endl;
  for (const auto &filePath: totalFiles) {
    std::array<unsigned char, 64> FileID{};
    std::array<unsigned char, 64> EncryptionID{};
    auto wch = filePath.wstring().c_str();
    if (fileMarkDll.extractIDsFromFile(wch, FileID.data(), EncryptionID.data())) {
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

std::vector<FileData> HelperUtils::findFileDataStructsInDatabase(const std::vector<FileIDData> &lostFiles) {
  RestApiDLL restApiDll;
  std::vector<FileData> fileDataList;

  if (printDebug) std::cout << "repairLostEncFileStructs: Finding FileData structs in database" << std::endl;
  for (const auto &FileIDDataStruct: lostFiles) {
    debugFileIDData(FileIDDataStruct);
    FileData fileData{};
    fileData.setFileId(new unsigned char[FileIDDataStruct.FileID.size()]);
    std::copy(FileIDDataStruct.FileID.begin(), FileIDDataStruct.FileID.end(), fileData.getFileId());
    fileData.setFileIdLength(FileIDDataStruct.FileID.size());

    fileData.setEncryptionId(new unsigned char[FileIDDataStruct.EncryptionID.size()]);
    std::copy(FileIDDataStruct.EncryptionID.begin(), FileIDDataStruct.EncryptionID.end(), fileData.getEncryptionId());
    fileData.setEncryptionIdLength(FileIDDataStruct.EncryptionID.size());

    if (fileData.getFileId() == nullptr || fileData.getEncryptionId() == nullptr) {
      std::cerr << "repairLostEncFileStructs: FileID or EncryptionID is null" << std::endl;
      continue;
    } else {
      if (printDebug) {
        std::cout << "repairLostEncFileStructs: FileID: "
                  << globalDefinitions::toHexString(fileData.getFileId(), fileData.getFileIdLength()) << std::endl;
        std::cout << "repairLostEncFileStructs: EncryptionID: "
                  << globalDefinitions::toHexString(fileData.getEncryptionId(), fileData.getEncryptionIdLength()) << std::endl;
      }
    }

    if (restApiDll.SearchEntry(fileData)) {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: Found struct for file: " << FileIDDataStruct.newEncryptedFilePath
                  << std::endl;
      fileDataList.push_back(fileData);
      if (printDebug) {
        std::cout << "repairLostEncFileStructs: FileID: "
                  << globalDefinitions::toHexString(fileData.getFileId(), fileData.getFileIdLength()) << std::endl;
        std::cout << "repairLostEncFileStructs: EncryptionID: "
                  << globalDefinitions::toHexString(fileData.getEncryptionId(), fileData.getEncryptionIdLength()) << std::endl;
      }
    } else {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: Could not find struct for file: "
                  << FileIDDataStruct.newEncryptedFilePath << std::endl;
    }
  }
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Found " << fileDataList.size() << " FileData structs in the db"
              << std::endl;

  return fileDataList;
}

void HelperUtils::updateFileDataInDB(std::vector<FileData> &fileDataList, const std::vector<FileIDData> &lostFiles, unsigned char *updateId) {
  RestApiDLL restApiDll;

  if (printDebug)
    std::cout << "repairLostEncFileStructs: Updating EncryptedFilePath in the FileData structs" << std::endl;
  for (auto &fileData: fileDataList) {
    if (fileData.getFileId() == nullptr || fileData.getEncryptionId() == nullptr) {
      if (printDebug) std::cout << "repairLostEncFileStructs: FileID or EncryptionID is null" << std::endl;
      continue;
    }
    bool updated = false;
    for (const auto &lostFile: lostFiles) {
      if (printDebug) std::cout << "repairLostEncFileStructs: Comparing FileIDs and EncryptionIDs" << std::endl;
      if (printDebug) {
        std::cout << "repairLostEncFileStructs: LostFileID: "
                  << globalDefinitions::toHexString(lostFile.FileID.data(), lostFile.FileID.size()) << std::endl;
        std::cout << "repairLostEncFileStructs: FileID: "
                  << globalDefinitions::toHexString(fileData.getFileId(), fileData.getFileIdLength()) << std::endl;
        std::cout << "repairLostEncFileStructs: LostEncryptionID: "
                  << globalDefinitions::toHexString(lostFile.EncryptionID.data(), lostFile.EncryptionID.size()) << std::endl;
        std::cout << "repairLostEncFileStructs: EncryptionID: "
                  << globalDefinitions::toHexString(fileData.getEncryptionId(), fileData.getEncryptionIdLength()) << std::endl;
      }

      bool equal = true;
      for (size_t i = 0; i < lostFile.FileID.size(); i++) {
        if (lostFile.FileID[i] != fileData.getFileId()[i] || lostFile.EncryptionID[i] != fileData.getEncryptionId()[i]) {
          equal = false;
          if (printDebug) std::cout << "repairLostEncFileStructs: FileIDs or EncryptionIDs do not match" << std::endl;
          break;
        }
      }

      if (equal) {
        if (printDebug) std::cout << "repairLostEncFileStructs: FileIDs and EncryptionIDs match" << std::endl;

        if (printDebug)
          std::cout << "repairLostEncFileStructs: Allocating memory for new EncryptedFilePath" << std::endl;
        fileData.setEncryptedFilePath(new wchar_t[lostFile.newEncryptedFilePath.wstring().size() + 1]);

        if (printDebug) std::cout << "repairLostEncFileStructs: Copying new EncryptedFilePath" << std::endl;
        if (printDebug)
          std::cout << "repairLostEncFileStructs: newEncryptedFilePath = "
                    << const_cast<wchar_t *>(lostFile.newEncryptedFilePath.c_str()) << std::endl;

        fileData.setEncryptedFilePath(const_cast<wchar_t *>(lostFile.newEncryptedFilePath.c_str()));
        fileData.setLastUpdateId(updateId);

        if (printDebug) std::cout << "repairLostEncFileStructs: Updated EncryptedFilePath" << std::endl;
        // Replace the old FileData struct in the database with the updated one
        if (restApiDll.ReplaceEntry(fileData)) {
          std::wcout << L"repairLostEncFileStructs: Replaced struct for file: " << fileData.getEncryptedFilePath()
                     << std::endl;
          updated = true;
        } else {
          std::wcout << L"repairLostEncFileStructs: Could not replace struct for file: "
                     << fileData.getEncryptedFilePath() << std::endl;
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
    std::cout << "repairLostEncFileStructs: Updated " << fileDataList.size() << " FileData structs in the db"
              << std::endl;
}

void HelperUtils::repairAllLostStruct() {
  auto drives = SystemUtils::getAllDrives();
  repairLostEncFileStructs(drives);
}