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
#include "gui/FileVaultGui.h"
#include <QApplication>

namespace fs = std::filesystem;

bool printDebug = true;
bool printConverterDebug = true;

#define pathToCrypt "S:\\clips\\cut"
#define dockerHash "a5d0aa35ab0c107e7a04e4618a9fe80c1c6ddb4eeef2e03fd2114fcb7e8be62a"

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
    partialStruct.setEncryptionId(new unsigned char[64]);
    partialStruct.setEncryptionIdLength(64);
    partialStruct.setEncryptedFilePath(StructUtils::ConvertWStringToWChar(filePath.wstring()));

    if (fileMarkDll.extractIDsFromFile(partialStruct.getEncryptedFilePath(), partialStruct.getFileId(),
                                       partialStruct.getEncryptionId())) {
      if (printDebug)
        std::cout << "++Extracted FileID: "
                  << globalDefinitions::toHexString(partialStruct.getFileId(), partialStruct.getFileIdLength())
                  << " from: "
                  << filePath << "++" << std::endl;
      std::cout << "++Extracted EncryptionID: "
                << globalDefinitions::toHexString(partialStruct.getEncryptionId(),
                                                  partialStruct.getEncryptionIdLength())
                << " from: "
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
      std::wcout << L"Try deleting file: " << fileData.getDecryptedFilePath() << std::endl;
      fileData.cleanupFileData();
    } else {
      std::wcout << L"++Failed to delete struct for file: " << fileData.getOriginalFilePath() << "++" << std::endl;
    }
  }
  std::cout << "###############################################################################################"
            << std::endl;
}

void testRun(int testRuns) {
  startDBContainer();
  FileScannerDLL fileScannerDll;
  CryptoDLL cryptoDll;
  RestApiDLL restApiDll;
  FileMarkDLL fileMarkDll;
  std::vector<FileData> fileDataVec;

  int testedFileCount = 0;
  int succeededFileCount = 0;
  int failedFileCount = 0;

  for (int i = 0; i < testRuns; ++i) {
    system(".\\RustFileCopy.exe");
    scanAndBuildStructs(fileScannerDll, fileDataVec);
    encryptAndSaveFiles(cryptoDll, restApiDll, fileDataVec);
    repairAndReloadFiles(fileScannerDll, fileMarkDll, restApiDll, fileDataVec);
    decryptAndDeleteFiles(cryptoDll, restApiDll, fileDataVec);

    auto encryptedFile = fileScannerDll.ScanDirectory(pathToCrypt, false);
    auto decryptedFile = fileScannerDll.ScanDirectory(pathToCrypt, true);

    testedFileCount += (encryptedFile.size() + decryptedFile.size());
    succeededFileCount += encryptedFile.size();
    failedFileCount += decryptedFile.size();
  }

  std::cout.flush();
  std::cout << "Test on: " << testRuns << " runs" << std::endl;
  std::cout << "Total files: " << testedFileCount << std::endl;
  std::cout << "Succeeded files: " << succeededFileCount - failedFileCount << std::endl;
  std::cout << "Failed files: " << failedFileCount << std::endl;
  std::cout << "Success rate: " << (static_cast<double>(testedFileCount - failedFileCount) / testedFileCount) * 100
            << "%" << std::endl;
  stopDBContainer();

  std::vector<int> result;
  result.push_back(testedFileCount);
  result.push_back(succeededFileCount);
  result.push_back(failedFileCount);
}

int main(int argc, char *argv[]) {
 /*int testRuns = 20;
  testRun(testRuns);
  return 0;*/

  QApplication app(argc, argv);

  FileVaultGui fileVaultGui;
  fileVaultGui.setWindowIcon(QIcon(":/icons/Icon.png"));
  fileVaultGui.show();

  return app.exec();
}