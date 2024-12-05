#include "StructUtils/StructUtils.h"
#include "HelperUtils/HelperUtils.h"
#include <filesystem>
#include <iostream>
#include <string>
#include "../DLLUtils/FileScannerDLL.h"
#include "../DLLUtils/MultiCryptoDLL.h"
#include "../DLLUtils/RestApiDLL.h"
#include "../DLLUtils/FileMarkDLL.h"

namespace fs = std::filesystem;

bool printDebug = true;
bool printConverterDebug = true;

#define pathToCrypt "C:\\hallo"

void startDBContainer() {
  std::string command = "docker start 4b3b761b6489ccb3f3f6a643bc10b1edc1459902a85977fbf99c0e64db571197";
  system(command.c_str());
}

void stopDBContainer() {
  std::string command = "docker stop 4b3b761b6489ccb3f3f6a643bc10b1edc1459902a85977fbf99c0e64db571197";
  system(command.c_str());
}

void removeFailedEntries(std::vector<FileData> &fileDataVec, std::vector<bool> &results) {
  for (size_t i = 0; i < results.size();) {
    if (!results[i]) {
      fileDataVec.erase(fileDataVec.begin() + i);
      results.erase(results.begin() + i);
    } else {
      ++i;
    }
  }
}

void scanAndBuildStructs(FileScannerDLL &fileScannerDll, std::vector<FileData> &fileDataVec) {
  if (printDebug) std::cout << "--Starting file scan--" << std::endl;
  auto pathList = fileScannerDll.ScanDirectory(pathToCrypt, false);
  if (printDebug) std::cout << "++Scan completed: found " << pathList.size() << " files!++" << std::endl;

  if (!pathList.empty()) {
    if (printDebug) std::cout << "--Building Structs--" << std::endl;
    for (const auto &filePath: pathList) {
      auto filePathStr = filePath.wstring();
      if (filePathStr.empty() || filePathStr.find(L'\\') == std::wstring::npos) continue;

      std::wcout << L"++Generating Struct for path: " << filePathStr << "++" << std::endl;
      fileDataVec.push_back(
          StructUtils::createFileDataStruct(globalDefinitions::AlgorithmType::AES256, fs::path(filePath)));
    }
  }
}

void encryptAndSaveFiles(MultiCryptoDLL &multiCryptoDll, RestApiDLL &restApiDll, std::vector<FileData> &fileDataVec) {
  if (fileDataVec.empty()) return;

  if (printDebug) std::cout << "--Encrypting Files--" << std::endl;
  std::vector<bool> encryptResults(fileDataVec.size(), false);
  multiCryptoDll.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), encryptResults);

  removeFailedEntries(fileDataVec, encryptResults);
  if (printDebug) std::cout << "++" << fileDataVec.size() << " Files Encrypted++" << std::endl;

  if (printDebug) std::cout << "--Saving File Data--" << std::endl;
  for (auto &fileData: fileDataVec) {
    if (restApiDll.InsertEntry(fileData)) {
      if (printConverterDebug) globalDefinitions::debugFileData(fileData);
      std::wcout << L"++Saved struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
      globalDefinitions::cleanupFileData(fileData);
    } else {
      std::wcout << L"++Failed to save struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
    }
  }
}

void repairAndReloadFiles(FileScannerDLL &fileScannerDll, FileMarkDLL &fileMarkDll, RestApiDLL &restApiDll,
                          std::vector<FileData> &fileDataVec) {
  if (printDebug) std::cout << "--Repairing Lost Encrypted File Structs--" << std::endl;
  HelperUtils helperUtils;
  std::vector<fs::path> paths = {pathToCrypt};
  helperUtils.repairLostEncFileStructs(paths);

  fileDataVec.clear();
  if (printDebug) std::cout << "--Rescanning Directory for encrypted files--" << std::endl;
  auto pathList = fileScannerDll.ScanDirectory(pathToCrypt, true);
  if (printDebug) std::cout << "++Scan completed: found " << pathList.size() << " files!++" << std::endl;

  if (printDebug) std::cout << "--Reloading File Data--" << std::endl;
  for (const auto &filePath: pathList) {
    FileData partialStruct{};
    partialStruct.FileID = new unsigned char[64];
    partialStruct.fileIDLength = 64;
    partialStruct.EncryptedFilePath = StructUtils::ConvertWStringToWChar(filePath.wstring());

    if (fileMarkDll.ExtractFileIDFromFile(partialStruct.EncryptedFilePath, partialStruct.FileID)) {
      if (printDebug)
        std::cout << "++Extracted FileID: "
                  << globalDefinitions::toHexString(partialStruct.FileID, partialStruct.fileIDLength) << " from: "
                  << filePath << "++" << std::endl;
    } else {
      std::wcout << L"++Failed to extract File ID from: " << filePath << "++" << std::endl;
      delete[] partialStruct.FileID;
      delete[] partialStruct.EncryptedFilePath;
      continue;
    }

    if (restApiDll.SearchEntry(partialStruct)) {
      fileDataVec.push_back(partialStruct);
      std::wcout << L"++Found and completed struct for file: " << filePath << "++" << std::endl;
    } else {
      std::wcout << L"++Could not find struct for file: " << filePath << " in database++" << std::endl;
    }

  }
}

void decryptAndDeleteFiles(MultiCryptoDLL &multiCryptoDll, RestApiDLL &restApiDll, std::vector<FileData> &fileDataVec) {
  if (fileDataVec.empty()) return;

  if (printDebug) std::cout << "--Decrypting Files--" << std::endl;
  std::vector<bool> decryptResults(fileDataVec.size(), false);
  multiCryptoDll.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), decryptResults);

  removeFailedEntries(fileDataVec, decryptResults);
  if (printDebug) std::cout << "++" << fileDataVec.size() << " Files Decrypted++" << std::endl;

  if (printDebug) std::cout << "--Deleting Structs--" << std::endl;

  for (auto &fileData: fileDataVec) {
    if (restApiDll.DeleteEntry(fileData)) {
      std::wcout << L"++Deleted struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
    } else {
      std::wcout << L"++Failed to delete struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
    }
    globalDefinitions::cleanupFileData(fileData);
  }
}

void testRun() {
  FileScannerDLL fileScannerDll;
  MultiCryptoDLL multiCryptoDll;
  RestApiDLL restApiDll;
  FileMarkDLL fileMarkDll;
  std::vector<FileData> fileDataVec;

  scanAndBuildStructs(fileScannerDll, fileDataVec);
  encryptAndSaveFiles(multiCryptoDll, restApiDll, fileDataVec);
  repairAndReloadFiles(fileScannerDll, fileMarkDll, restApiDll, fileDataVec);
  decryptAndDeleteFiles(multiCryptoDll, restApiDll, fileDataVec);
}

int main() {
  startDBContainer();
  testRun();
  stopDBContainer();
  return 0;
}