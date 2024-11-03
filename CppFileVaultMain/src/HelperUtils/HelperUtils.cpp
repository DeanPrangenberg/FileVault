#include <vector>
#include <locale>
#include <codecvt>
#include <array>
#include <iostream>
#include "HelperUtils.h"
#include "../DLLUtils/DLLUtils.h"
#include "../System/SystemUtils.h"

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
  auto dllUtils = DLLUtils();
  std::vector<fs::path> totalFiles;

  // Scanning for all files in all directories
  std::cout << "Scanning for all files" << std::endl;
  for (auto &directory: directorys) {
    auto foundFiles = dllUtils.ScanDirectory(directory, true);
    totalFiles.insert(totalFiles.end(), foundFiles.begin(), foundFiles.end());
  }
  std::cout << "Found " << totalFiles.size() << " \"encrypted\" files" << std::endl;

  // Checking files for fileID
  std::cout << "Checking files for FileID" << std::endl;
  std::vector<FileIDData> lostFiles;
  for (const auto &filePath: totalFiles) {
    std::array<unsigned char, 64> FileID;
    auto wch = ConvertWStringToWChar(filePath.wstring());
    if (dllUtils.ExtractFileIDFromFile(wch, FileID.data())) {
      std::cout << "FileID found for file: " << filePath << std::endl;
      lostFiles.push_back({FileID, filePath});
    }
    delete[] wch; // Free allocated memory
  }
  std::cout << "Found " << lostFiles.size() << " files with FileID" << std::endl;

  // Find filedata structs in json file
  std::cout << "Finding FileData structs in json file" << std::endl;
  std::vector<globalDefinitions::FileData> fileDataList;
  for (const auto &FileIDDataStruct: lostFiles) {
    FileData fileData;
    fileData.FileID = new unsigned char[64];
    std::copy(FileIDDataStruct.FileID.begin(), FileIDDataStruct.FileID.end(), fileData.FileID);
    fileData.fileIDLength = FileIDDataStruct.FileID.size();

    if (dllUtils.FindAndCompleteStruct(&fileData, globalDefinitions::jsonFileName)) {
      fileDataList.push_back(fileData);
    } else {
      delete[] fileData.FileID;
    }
  }
  std::cout << "Found " << fileDataList.size() << " FileData structs in the json" << std::endl;

  // Update the EncryptedFilePath in the FileData structs
  std::cout << "Updating EncryptedFilePath in the FileData structs" << std::endl;
  for (auto &fileData : fileDataList) {
    for (const auto &lostFile : lostFiles) {
      if (std::vector<unsigned char>(fileData.FileID, fileData.FileID + fileData.fileIDLength) == std::vector<unsigned char>(lostFile.FileID.begin(), lostFile.FileID.end())) {
        auto wch = ConvertWStringToWChar(lostFile.newEncryptedFilePath.wstring());
        fileData.EncryptedFilePath = new wchar_t[wcslen(wch) + 1];
        std::wmemcpy(const_cast<wchar_t *>(fileData.EncryptedFilePath), wch, wcslen(wch) + 1);
        delete[] wch; // Free allocated memory
        break;
      }
    }
  }

  // Delete old FileData structs from json file
  std::cout << "Deleting old FileData structs from json file" << std::endl;
  for (const auto &fileData: fileDataList) {
    dllUtils.DeleteStructFromJson(&fileData, globalDefinitions::jsonFileName);
  }

  // Add new FileData structs to json file
  std::cout << "Adding new FileData structs to json file" << std::endl;
  dllUtils.WriteFileDataToJson(fileDataList);

  // Free allocated memory
  std::cout << "Freeing allocated memory" << std::endl;
  for (auto &fileData : fileDataList) {
    delete[] fileData.FileID;
    delete[] fileData.EncryptedFilePath;
  }
  std::cout << "Memory freed, repairLostEncFileStructs done!" << std::endl;
}

void HelperUtils::repairAllLostStruct() {
  auto drives = SystemUtils::getAllDrives();
  repairLostEncFileStructs(drives);
}