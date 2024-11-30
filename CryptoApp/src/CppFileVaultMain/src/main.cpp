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

#define pathToCrypt "S:\\clips\\cut"

void startDBContainer() {
  std::string command = "docker start 2d73a4997f4068f1f0c2009d284225bbc1f265d78805043db61a63054e7cddd2";
  system(command.c_str());
}

void stopDBContainer() {
  std::string command = "docker stop 2d73a4997f4068f1f0c2009d284225bbc1f265d78805043db61a63054e7cddd2";
  system(command.c_str());
}

void testRun() {
  FileScannerDLL fileScannerDll;
  MultiCryptoDLL multiCryptoDll;
  RestApiDLL restApiDll;
  FileMarkDLL fileMarkDll;

  if (printDebug) std::cout << "--Starting file scan--" << std::endl;
  auto pathList = fileScannerDll.ScanDirectory(pathToCrypt, false);
  if (printDebug) std::cout << "++Scan completed found " << pathList.size() << " files!++" << std::endl;

  std::vector<FileData> fileDataVec;
  if (!pathList.empty()) {
    if (printDebug) std::cout << "--Building Structs--" << std::endl;
    for (const auto &filePath: pathList) {
      auto filePathstr = filePath.wstring();

      if (filePathstr.empty() || filePathstr.find('\\') == std::string::npos) {
        continue;
      }

      std::wcout << L"++Generating Struct for path: " << filePathstr << "++" << std::endl;
      fileDataVec.push_back(
          StructUtils::createFileDataStruct(globalDefinitions::AlgorithmType::AES256, fs::path(filePath)));
    }

    if (!fileDataVec.empty()) {
      if (printDebug) std::cout << "--Encrypting Files--" << std::endl;
      std::vector<bool> encryptResults(fileDataVec.size(), false);
      multiCryptoDll.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), encryptResults);

      for (int i = 0; i < encryptResults.size(); i++) {
        if (!encryptResults[i]) {
          fileDataVec.erase(fileDataVec.begin() + i);
          encryptResults.erase(encryptResults.begin() + i);
          i--; // Adjust index after erasing
        }
      }

      if (printDebug) std::cout << "++" << fileDataVec.size() << " Files Encrypted++" << std::endl;

      if (printDebug) std::cout << "--Saving File Data--" << std::endl;
      for (const auto &fileData: fileDataVec) {
        if (restApiDll.InsertEntry(fileData)) {
          if (printConverterDebug) globalDefinitions::debugFileData(fileData);
          std::wcout << L"++Saved struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
        } else {
          std::wcout << L"++Failed to save struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
        }
      }

      std::cin.get();

      if (printDebug) std::cout << "--Repairing Lost Encrypted File Structs--" << std::endl;
      auto helperUtils = HelperUtils();
      std::vector<fs::path> paths = {pathToCrypt};
      helperUtils.repairLostEncFileStructs(paths);

      if (printDebug) std::cout << "--Back in main after repair--" << std::endl;

      // Clear the current fileDataVec and rescan the directory
      fileDataVec.clear();
      if (printDebug) std::cout << "--Rescanning Directory for encrypted files--" << std::endl;
      pathList = fileScannerDll.ScanDirectory(pathToCrypt, true);
      if (printDebug) std::cout << "++Scan completed found " << pathList.size() << " files!++" << std::endl;

      // Reload the FileData structs from the database
      if (printDebug) std::cout << "--Reloading File Data--" << std::endl;
      for (const auto &filePath: pathList) {
        FileData partialStruct;
        partialStruct.FileID = new unsigned char[64];
        partialStruct.fileIDLength = 64;
        partialStruct.EncryptedFilePath = StructUtils::ConvertWStringToWChar(filePath.wstring());

        if (fileMarkDll.ExtractFileIDFromFile(partialStruct.EncryptedFilePath, partialStruct.FileID)) {
          if (printDebug) std::cout << "++Extracted FileID: "<< globalDefinitions::toHexString(partialStruct.FileID, partialStruct.fileIDLength) <<" from: " << filePath << "++" << std::endl;
        } else {
          if (printDebug) std::cout << "++Could not extract File ID from: " << filePath << "++" << std::endl;
          delete[] partialStruct.EncryptedFilePath;
          delete[] partialStruct.FileID;
          continue;
        }

        if (restApiDll.SearchEntry(partialStruct)) {
          fileDataVec.push_back(partialStruct);
          std::wcout << L"++Found and completed struct for file: " << filePath << "++" << std::endl;
        } else {
          if (partialStruct.EncryptedFilePath) {
            std::wcout << L"++Could not find struct for: " << filePath << " in database++" << std::endl;
          } else {
            std::wcout << L"++Could not find struct for the file in database++" << std::endl;
          }
        }
      }
      if (printDebug) std::cout << "++Completed " << fileDataVec.size() << " Structs from Save File++" << std::endl;

      for (const auto fileData: fileDataVec) {
        std::wcout << L"++FileData Struct: " << fileData.EncryptedFilePath << "++" << std::endl;
        if (printConverterDebug) globalDefinitions::debugFileData(fileData);
      }

      if (printDebug) std::cout << "--Decrypting Files--" << std::endl;
      std::vector<bool> decryptResults(fileDataVec.size(), false);
      multiCryptoDll.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), decryptResults);

      for (int i = 0; i < decryptResults.size(); i++) {
        if (!decryptResults[i]) {
          fileDataVec.erase(fileDataVec.begin() + i);
          decryptResults.erase(decryptResults.begin() + i);
          i--; // Adjust index after erasing
        }
      }
      if (printDebug) std::cout << "++" << fileDataVec.size() << " Files Decrypted++" << std::endl;

      if (printDebug) std::cout << "--Deleting Structs--" << std::endl;
      for (const auto &fileData: fileDataVec) {
        if (restApiDll.DeleteEntry(fileData)) {
          std::wcout << L"++Deleted struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
        }
      }
    }
  }
}

int main() {
  startDBContainer();
  testRun();
  stopDBContainer();
  return 0;
}