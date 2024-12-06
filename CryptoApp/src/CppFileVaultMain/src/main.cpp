#include "StructUtils/StructUtils.h"
#include "HelperUtils/HelperUtils.h"
#include <filesystem>
#include <iostream>
#include <string>
#include "../DLLUtils/FileScannerDLL.h"
#include "../DLLUtils/CryptoDLL.h"
#include "../DLLUtils/RestApiDLL.h"
#include "../DLLUtils/FileMarkDLL.h"
#include "../../shared/FileData.h"

namespace fs = std::filesystem;

bool printDebug = true;
bool printConverterDebug = true;

#define pathToCrypt "S:\\clips\\cut"
#define dockerHash "99081b4ebcbdde16ab0ce20bf701d7461622894d07dac3b0c938b744f865e13c"

void startDBContainer() {
  std::string command = "docker start " + std::string(dockerHash);
  system(command.c_str());
}

void stopDBContainer() {
  std::string command = "docker stop " + std::string(dockerHash);
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

void printDiv() {
  for (int k = 0; k < 3; ++k) {
    for (int i = 0; i < 90; ++i) {
      std::cout << "#";
    }
    std::cout << std::endl;
  }
}

void scanAndBuildStructs(FileScannerDLL &fileScannerDll, std::vector<FileData> &fileDataVec) {
  printDiv();
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
  printDiv();
}

void encryptAndSaveFiles(CryptoDLL &cryptoDll, RestApiDLL &restApiDll, std::vector<FileData> &fileDataVec) {
  if (fileDataVec.empty()) return;

  if (printDebug) std::cout << "--Encrypting Files--" << std::endl;
  std::vector<bool> encryptResults(fileDataVec.size(), false);
  cryptoDll.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), encryptResults);

  removeFailedEntries(fileDataVec, encryptResults);
  if (printDebug) std::cout << "++" << fileDataVec.size() << " Files Encrypted++" << std::endl;

  if (printDebug) std::cout << "--Saving File Data--" << std::endl;
  for (auto &fileData: fileDataVec) {
    if (restApiDll.InsertEntry(fileData)) {
      if (printConverterDebug) fileData.debugFileData();
      std::wcout << L"++Saved struct for file: " << fileData.getOriginalFilePath() << "++" << std::endl;
      fileData.cleanupFileData();
    } else {
      std::wcout << L"++Failed to save struct for file: " << fileData.getOriginalFilePath() << "++" << std::endl;
    }
  }
}

void repairAndReloadFiles(FileScannerDLL &fileScannerDll, FileMarkDLL &fileMarkDll, RestApiDLL &restApiDll,
                          std::vector<FileData> &fileDataVec) {
  printDiv();
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
    partialStruct.setFileId(new unsigned char[64]);
    partialStruct.setFileIdLength(64);
    partialStruct.setEncryptedFilePath(StructUtils::ConvertWStringToWChar(filePath.wstring()));

    if (fileMarkDll.ExtractFileIDFromFile(partialStruct.getEncryptedFilePath(), partialStruct.getFileId())) {
      if (printDebug)
        std::cout << "++Extracted FileID: "
                  << globalDefinitions::toHexString(partialStruct.getFileId(), partialStruct.getFileIdLength()) << " from: "
                  << filePath << "++" << std::endl;
    } else {
      std::wcout << L"++Failed to extract File ID from: " << filePath << "++" << std::endl;
      partialStruct.cleanupFileData();
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

void decryptAndDeleteFiles(CryptoDLL &cryptoDll, RestApiDLL &restApiDll, std::vector<FileData> &fileDataVec) {
  printDiv();
  if (fileDataVec.empty()) return;

  if (printDebug) std::cout << "--Decrypting Files--" << std::endl;
  std::vector<bool> decryptResults(fileDataVec.size(), false);
  cryptoDll.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), decryptResults);

  removeFailedEntries(fileDataVec, decryptResults);
  if (printDebug) std::cout << "++" << fileDataVec.size() << " Files Decrypted++" << std::endl;

  if (printDebug) std::cout << "--Deleting Structs--" << std::endl;

  for (auto &fileData: fileDataVec) {
    if (restApiDll.DeleteEntry(fileData)) {
      std::wcout << L"++Deleted struct for file: " << fileData.getOriginalFilePath() << "++" << std::endl;
    } else {
      std::wcout << L"++Failed to delete struct for file: " << fileData.getOriginalFilePath() << "++" << std::endl;
    }

    try {
      std::wcout << L"Try deleting file: " << fileData.getDecryptedFilePath() << std::endl;
      fileData.cleanupFileData();
    } catch (const std::exception &e) {
      std::wcerr << L"Failed to clean fileData: " << L" -> " << e.what() << std::endl;
    }
  }
}

std::vector<int> testRun() {
  FileScannerDLL fileScannerDll;
  CryptoDLL cryptoDll;
  RestApiDLL restApiDll;
  FileMarkDLL fileMarkDll;
  std::vector<FileData> fileDataVec;

  scanAndBuildStructs(fileScannerDll, fileDataVec);
  encryptAndSaveFiles(cryptoDll, restApiDll, fileDataVec);
  repairAndReloadFiles(fileScannerDll, fileMarkDll, restApiDll, fileDataVec);
  decryptAndDeleteFiles(cryptoDll, restApiDll, fileDataVec);

  std::vector<int> result;
  auto encryptedFile = fileScannerDll.ScanDirectory(pathToCrypt, false);
  auto decryptedFile = fileScannerDll.ScanDirectory(pathToCrypt, true);

  result.push_back(encryptedFile.size());
  result.push_back(decryptedFile.size());
}

int main() {
  int testRuns = 10;
  int testedFileCount = 0;
  int failedFileCount = 0;

  for (int i = 0; i < testRuns; ++i) {
    startDBContainer();
    system(".\\RustFileCopy.exe");
    auto result = testRun();
    testedFileCount += result[0] + result[1];
    failedFileCount += result[1];
    stopDBContainer();
  }
  std::cout << "###############################################################################################" << std::endl;
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "###############################################################################################" << std::endl;
  std::cout << std::endl;
  std::cout << "Tested " << testedFileCount << " files with " << failedFileCount << " failed files" << std::endl;
  std::cout << std::endl;
  std::cout << "###############################################################################################" << std::endl;
  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "###############################################################################################" << std::endl;

  return 0;
}