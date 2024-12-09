#include <vector>
#include <array>
#include <iostream>
#include "HelperUtils.h"
#include "../System/SystemUtils.h"
#include "../DLLUtils/FileScannerDLL.h"
#include "../DLLUtils/RestApiDLL.h"
#include "../DLLUtils/FileMarkDLL.h"

struct FileIDData {
  std::array<unsigned char, 64> FileID;
  fs::path newEncryptedFilePath;
};

void debugFileIDData(FileIDData fileIDData) {
  std::cout << "***************************************************" << std::endl;
  std::cout << "FileID: ";
  for (const auto &byte: fileIDData.FileID) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }

  std::cout << std::dec;

  std::cout << std::endl;

  std::cout << "newEncryptedFilePath :" << fileIDData.newEncryptedFilePath << std::endl;

  std::cout << "***************************************************" << std::endl;
}

wchar_t *HelperUtils::ConvertWStringToWChar(const std::wstring &wstr) {
  auto *wch = new wchar_t[wstr.size() + 1]; // Allocate memory
  std::wmemcpy(wch, wstr.c_str(), wstr.size() + 1); // Copy the contents
  return wch;
}

void HelperUtils::repairLostEncFileStructs(std::vector<fs::path> &directorys) {
  FileScannerDLL fileScannerDll;
  RestApiDLL restApiDll;
  FileMarkDLL fileMarkDll;

  std::vector<fs::path> totalFiles;

  // Scanning for all files in all directories
  if (printDebug) std::cout << "repairLostEncFileStructs: Scanning for all files" << std::endl;
  for (auto &directory: directorys) {
    auto foundFiles = fileScannerDll.ScanDirectory(directory, true);
    totalFiles.insert(totalFiles.end(), foundFiles.begin(), foundFiles.end());
  }
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Found " << totalFiles.size() << " encrypted files" << std::endl;

  // Checking files for fileID
  if (printDebug) std::cout << "repairLostEncFileStructs: Checking files for FileID" << std::endl;
  std::vector<FileIDData> lostFiles;
  for (const auto &filePath: totalFiles) {
    std::array<unsigned char, 64> FileID{};
    auto wch = ConvertWStringToWChar(filePath.wstring());
    if (fileMarkDll.ExtractFileIDFromFile(wch, FileID.data())) {
      if (printDebug) std::cout << "FileID found for file: " << filePath << std::endl;
      lostFiles.push_back({FileID, filePath});
    } else {
      std::cerr << "Failed to extract FileID for file: " << filePath << std::endl;
    }
    delete[] wch; // Free allocated memory
    wch = nullptr;
  }
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Found " << lostFiles.size() << " lost files with FileID" << std::endl;

  // Find filedata structs in database
  if (printDebug) std::cout << "repairLostEncFileStructs: Finding FileData structs in database" << std::endl;
  std::vector<FileData> fileDataList;
  for (const auto &FileIDDataStruct: lostFiles) {
    debugFileIDData(FileIDDataStruct);
    FileData fileData{};
    fileData.setFileId(new unsigned char[FileIDDataStruct.FileID.size()]);
    std::copy(FileIDDataStruct.FileID.begin(), FileIDDataStruct.FileID.end(), fileData.getFileId());
    fileData.setFileIdLength(FileIDDataStruct.FileID.size());

    if (fileData.getFileId() == nullptr) {
      std::cerr << "repairLostEncFileStructs: FileID is null" << std::endl;
      continue;
    } else {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: FileID: "
                  << globalDefinitions::toHexString(fileData.getFileId(), fileData.getFileIdLength()) << std::endl;
    }

    if (restApiDll.SearchEntry(fileData)) {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: Found struct for file: " << FileIDDataStruct.newEncryptedFilePath
                  << std::endl;
      fileDataList.push_back(fileData);
      if (printDebug)
        std::cout << "repairLostEncFileStructs: FileID: "
                  << globalDefinitions::toHexString(fileData.getFileId(), fileData.getFileIdLength()) << std::endl;
    } else {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: Could not find struct for file: "
                  << FileIDDataStruct.newEncryptedFilePath << std::endl;
    }
  }
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Found " << fileDataList.size() << " FileData structs in the db"
              << std::endl;

  // Update the EncryptedFilePath in the FileData structs
  if (printDebug)
    std::cout << "repairLostEncFileStructs: Updating EncryptedFilePath in the FileData structs" << std::endl;
  for (auto &fileData: fileDataList) {
    if (fileData.getFileId() == nullptr) {
      if (printDebug) std::cout << "repairLostEncFileStructs: FileID is null" << std::endl;
      continue;
    }
    bool updated = false;
    for (const auto &lostFile: lostFiles) {
      if (printDebug) std::cout << "repairLostEncFileStructs: Comparing FileIDs" << std::endl;
      if (printDebug)
        std::cout << "repairLostEncFileStructs: LostFileID: "
                  << globalDefinitions::toHexString(lostFile.FileID.data(), lostFile.FileID.size()) << std::endl;
      if (printDebug)
        std::cout << "repairLostEncFileStructs: FileID: "
                  << globalDefinitions::toHexString(fileData.getFileId(), fileData.getFileIdLength()) << std::endl;

      bool equal = true;
      for (size_t i = 0; i < lostFile.FileID.size(); i++) {
        if (lostFile.FileID[i] != fileData.getFileId()[i]) {
          equal = false;
          if (printDebug) std::cout << "repairLostEncFileStructs: FileIDs do not match" << std::endl;
          break;
        }
      }

      if (equal) {
        if (printDebug) std::cout << "repairLostEncFileStructs: FileIDs match" << std::endl;

        if (printDebug)
          std::cout << "repairLostEncFileStructs: Allocating memory for new EncryptedFilePath" << std::endl;
        fileData.setEncryptedFilePath(new wchar_t[lostFile.newEncryptedFilePath.wstring().size() + 1]);

        if (printDebug) std::cout << "repairLostEncFileStructs: Copying new EncryptedFilePath" << std::endl;
        if (printDebug)
          std::cout << "repairLostEncFileStructs: newEncryptedFilePath = "
                    << const_cast<wchar_t *>(lostFile.newEncryptedFilePath.c_str()) << std::endl;

        fileData.setEncryptedFilePath(const_cast<wchar_t *>(lostFile.newEncryptedFilePath.c_str()));

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
        if (printDebug) std::cout << "repairLostEncFileStructs: FileID and LostFileID do not match" << std::endl;
      }
    }
    if (!updated) {
      if (printDebug)
        std::cout << "repairLostEncFileStructs: Could not find a matching FileID for struct update ##" << std::endl;
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