#include <vector>
#include <locale>
#include <codecvt>
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

std::string HelperUtils::ConvertWStringToString(const std::wstring &wstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(wstr);
}

std::wstring HelperUtils::ConvertStringToWString(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.from_bytes(str);
}

std::string HelperUtils::ConvertVectorToString(const std::vector<unsigned char> &vec) {
  return std::string(vec.begin(), vec.end());
}

wchar_t *HelperUtils::ConvertWStringToWChar(const std::wstring &wstr) {
  wchar_t *wch = new wchar_t[wstr.size() + 1]; // Allocate memory
  std::wmemcpy(wch, wstr.c_str(), wstr.size() + 1); // Copy the contents
  return wch;
}

void HelperUtils::repairLostEncFileStructs(std::vector<fs::path> &directorys) {
  FileScannerDLL fileScannerDll;
  RestApiDLL restApiDll;
  FileMarkDLL fileMarkDll;

  std::vector<fs::path> totalFiles;

  // Scanning for all files in all directories
  std::cout << "repairLostEncFileStructs: Scanning for all files" << std::endl;
  for (auto &directory: directorys) {
    auto foundFiles = fileScannerDll.ScanDirectory(directory, true);
    totalFiles.insert(totalFiles.end(), foundFiles.begin(), foundFiles.end());
  }
  std::cout << "repairLostEncFileStructs: Found " << totalFiles.size() << " encrypted files" << std::endl;

  // Checking files for fileID
  std::cout << "repairLostEncFileStructs: Checking files for FileID" << std::endl;
  std::vector<FileIDData> lostFiles;
  for (const auto &filePath: totalFiles) {
    std::array<unsigned char, 64> FileID;
    auto wch = ConvertWStringToWChar(filePath.wstring());
    if (fileMarkDll.ExtractFileIDFromFile(wch, FileID.data())) {
      std::cout << "FileID found for file: " << filePath << std::endl;
      lostFiles.push_back({FileID, filePath});
    } else {
      std::cerr << "Failed to extract FileID for file: " << filePath << std::endl;
    }
    delete[] wch; // Free allocated memory
  }
  std::cout << "repairLostEncFileStructs: Found " << lostFiles.size() << " lost files with FileID" << std::endl;

  // Find filedata structs in database
  std::cout << "repairLostEncFileStructs: Finding FileData structs in database" << std::endl;
  std::vector<FileData> fileDataList;
  for (const auto &FileIDDataStruct: lostFiles) {
    FileData fileData{};
    fileData.FileID = const_cast<unsigned char *>(FileIDDataStruct.FileID.data());
    fileData.fileIDLength = FileIDDataStruct.FileID.size();

    if (fileData.FileID == nullptr) {
      std::cerr << "repairLostEncFileStructs: FileID is null" << std::endl;
      continue;
    } else {
      std::cout << "repairLostEncFileStructs: FileID: " << globalDefinitions::toHexString(fileData.FileID, fileData.fileIDLength) << std::endl;
    }

    if (restApiDll.SearchEntry(fileData)) {
      std::cout << "repairLostEncFileStructs: Found struct for file: " << FileIDDataStruct.newEncryptedFilePath << std::endl;
      fileDataList.push_back(fileData);
      std::cout << "repairLostEncFileStructs: FileID: " << globalDefinitions::toHexString(fileData.FileID, fileData.fileIDLength) << std::endl;
    } else {
      delete[] fileData.FileID;
      std::cout << "repairLostEncFileStructs: Could not find struct for file: " << FileIDDataStruct.newEncryptedFilePath << std::endl;
    }
  }
  std::cout << "repairLostEncFileStructs: Found " << fileDataList.size() << " FileData structs in the db" << std::endl;

  // Update the EncryptedFilePath in the FileData structs
  std::cout << "repairLostEncFileStructs: Updating EncryptedFilePath in the FileData structs" << std::endl;
  for (auto &fileData: fileDataList) {
    if (fileData.FileID == nullptr) {
      std::cout << "repairLostEncFileStructs: FileID is null" << std::endl;
      continue;
    }
    bool updated = false;
    for (const auto &lostFile: lostFiles) {
      std::cout << "repairLostEncFileStructs: Comparing FileIDs" << std::endl;
      std::cout << "repairLostEncFileStructs: LostFileID: " << globalDefinitions::toHexString(lostFile.FileID.data(), lostFile.FileID.size()) << std::endl;
      std::cout << "repairLostEncFileStructs: FileID: " << globalDefinitions::toHexString(fileData.FileID, fileData.fileIDLength) << std::endl;

      if (std::memcmp(lostFile.FileID.data(), fileData.FileID, 64)) {
        const auto wch = ConvertWStringToWChar(lostFile.newEncryptedFilePath.wstring());
        if (wch == nullptr) {
          std::cerr << "Failed to convert file path to wchar_t*" << std::endl;
          continue;
        }

        delete[] fileData.EncryptedFilePath; // Free old memory
        fileData.EncryptedFilePath = wch;

        // Replace the old FileData struct in the database with the updated one
        if (restApiDll.ReplaceEntry(fileData)) {
          std::wcout << L"repairLostEncFileStructs: Replaced struct for file: " << fileData.EncryptedFilePath << std::endl;
          updated = true;
        } else {
          std::wcout << L"repairLostEncFileStructs: Could not replace struct for file: " << fileData.EncryptedFilePath << std::endl;
        }

        break;
      } else {
        std::cout << "repairLostEncFileStructs: FileID and LostFileID do not match" << std::endl;
      }
    }
    if (!updated) {
      std::cout << "repairLostEncFileStructs: Could not find a matching FileID for struct update ##" << std::endl;
      auto it = std::remove(fileDataList.begin(), fileDataList.end(), fileData);
      if (it != fileDataList.end()) {
        fileDataList.erase(it, fileDataList.end());
        std::cout << "repairLostEncFileStructs: Removed struct from list" << std::endl;
      }
    }
  }

  std::cout << "repairLostEncFileStructs: Updated " << fileDataList.size() << " FileData structs in the db" << std::endl;
  // Free allocated memory
  for (auto &fileData: fileDataList) {
    delete[] fileData.FileID;
    delete[] fileData.EncryptedFilePath;
  }
  std::cout << "repairLostEncFileStructs: Finished" << std::endl;
}

void HelperUtils::repairAllLostStruct() {
  auto drives = SystemUtils::getAllDrives();
  repairLostEncFileStructs(drives);
}