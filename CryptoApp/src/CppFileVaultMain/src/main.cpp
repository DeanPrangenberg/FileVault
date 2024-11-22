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

#define pathToCrypt "S:\\clips\\cut"

void testRun() {
  FileScannerDLL fileScannerDll;
  MultiCryptoDLL multiCryptoDll;
  RestApiDLL restApiDll;
  FileMarkDLL fileMarkDll;

  std::cout << "--Starting file scan--" << std::endl;
  auto pathList = fileScannerDll.ScanDirectory(pathToCrypt, false);
  std::cout << "++Scan completed found " << pathList.size() << " files!++" << std::endl;

  std::vector<FileData> fileDataVec;
  if (!pathList.empty()) {
    std::cout << "--Building Structs--" << std::endl;
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
      std::cout << "--Encrypting Files--" << std::endl;
      std::vector<bool> encryptResults(fileDataVec.size(), false);
      multiCryptoDll.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), encryptResults);

      for (int i = 0; i < encryptResults.size(); i++) {
        if (!encryptResults[i]) {
          fileDataVec.erase(fileDataVec.begin() + i);
          encryptResults.erase(encryptResults.begin() + i);
          i--; // Adjust index after erasing
        }
      }

      std::cout << "++" << fileDataVec.size() << " Files Encrypted++" << std::endl;

      std::cout << "--Saving File Data--" << std::endl;
      for (const auto &fileData: fileDataVec) {
        if (restApiDll.InsertEntry(fileData)) {
          std::wcout << L"++Saved struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
        } else {
          std::wcout << L"++Failed to save struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
        }
      }

      std::cout << "--Repairing Lost Encrypted File Structs--" << std::endl;
      auto helperUtils = HelperUtils();
      std::vector<fs::path> paths = {pathToCrypt};
      helperUtils.repairLostEncFileStructs(paths);

      // Clear the current fileDataVec and rescan the directory
      fileDataVec.clear();
      std::cout << "--Rescanning Directory for encrypted files--" << std::endl;
      pathList = fileScannerDll.ScanDirectory(pathToCrypt, true);
      std::cout << "++Scan completed found " << pathList.size() << " files!++" << std::endl;

      // Reload the FileData structs from the database
      std::cout << "--Reloading File Data--" << std::endl;
      for (const auto &filePath: pathList) {
        FileData partialStruct;
        partialStruct.FileID = new unsigned char[64];
        partialStruct.EncryptedFilePath = StructUtils::ConvertWStringToWChar(filePath.wstring());

        if (fileMarkDll.ExtractFileIDFromFile(partialStruct.EncryptedFilePath, partialStruct.FileID)) {
          std::cout << "++Extracted File ID from: " << filePath << "++" << std::endl;
        } else {
          std::cout << "++Could not extract File ID from: " << filePath << "++" << std::endl;
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
      std::cout << "++Completed " << fileDataVec.size() << " Structs from Save File++" << std::endl;

      for (const auto fileData: fileDataVec) {
        std::wcout << L"++FileData Struct: " << fileData.EncryptedFilePath << "++" << std::endl;
        std::cout << "++FileID: ";
        for (int i = 0; i < fileData.fileIDLength; i++) {
          std::cout << fileData.FileID[i];
        }
        std::cout << std::endl;
      }

      std::cout << "--Decrypting Files--" << std::endl;
      std::vector<bool> decryptResults(fileDataVec.size(), false);
      multiCryptoDll.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), decryptResults);

      for (int i = 0; i < decryptResults.size(); i++) {
        if (!decryptResults[i]) {
          fileDataVec.erase(fileDataVec.begin() + i);
          decryptResults.erase(decryptResults.begin() + i);
          i--; // Adjust index after erasing
        }
      }
      std::cout << "++" << fileDataVec.size() << " Files Decrypted++" << std::endl;

      std::cout << "--Deleting Structs--" << std::endl;
      for (const auto &fileData: fileDataVec) {
        if (restApiDll.DeleteEntry(fileData)) {
          std::wcout << L"++Deleted struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
        }
      }
    }
  }
}

int main() {
  testRun();
  return 0;
}