#include "StructUtils/StructUtils.h"
#include "DLLUtils/DLLUtils.h"
#include "HelperUtils/HelperUtils.h"
#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

#define pathToCrypt "S:\\clips\\cut"

int main() {
  auto dllUtils = DLLUtils();

  std::cout << "--Starting file scan--" << std::endl;
  auto pathList = dllUtils.ScanDirectory(pathToCrypt, false);
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
      dllUtils.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), encryptResults);

      for (int i = 0; i < encryptResults.size(); i++) {
        if (!encryptResults[i]) {
          fileDataVec.erase(fileDataVec.begin() + i);
          encryptResults.erase(encryptResults.begin() + i);
          i--; // Adjust index after erasing
        }
      }

      std::cout << "++" << fileDataVec.size() << " Files Encrypted++" << std::endl;

      std::cout << "--Saving File Data--" << std::endl;
      dllUtils.WriteFileDataToJson(fileDataVec);
      std::cout << "Type something: ";

      int stop;
      std::cin >> stop;

      std::cout << "--Repairing Lost Encrypted File Structs--" << std::endl;
      auto helperUtils = HelperUtils();
      std::vector<fs::path> paths = {pathToCrypt};
      helperUtils.repairLostEncFileStructs(paths);

      // Clear the current fileDataVec and rescan the directory
      fileDataVec.clear();
      std::cout << "--Rescanning Directory for encrypted files--" << std::endl;
      pathList = dllUtils.ScanDirectory(pathToCrypt, true);
      std::cout << "++Scan completed found " << pathList.size() << " files!++" << std::endl;

      for (const auto &filePath: pathList) {
        std::wcout << filePath.wstring() << std::endl;
      }

      // Reload the FileData structs from the JSON file
      std::cout << "--Reloading File Data--" << std::endl;
      for (const auto &filePath: pathList) {
        auto *partialStruct = new FileData();
        partialStruct->FileID = new unsigned char[64];
        partialStruct->EncryptedFilePath = StructUtils::ConvertWStringToWChar(filePath.wstring());

        std::cout << "++Extracting File ID from: " << filePath << "++" << std::endl;

        dllUtils.ExtractFileIDFromFile(partialStruct->EncryptedFilePath, partialStruct->FileID);

        if (dllUtils.FindAndCompleteStruct(partialStruct, globalDefinitions::jsonFileName)) {
          fileDataVec.push_back(*partialStruct);
          std::wcout << L"++Found and completed struct for file: " << filePath << "++" << std::endl;
        } else {
          if (partialStruct->EncryptedFilePath) {
            std::wcout << L"++Could not find struct for: " << filePath << " in json file++" << std::endl;
          } else {
            std::wcout << L"++Could not find struct for the file in json file++" << std::endl;
          }
        }
      }
      std::cout << "++Completed " << fileDataVec.size() << " Structs from Save File++" << std::endl;

      for (const auto &fileData: fileDataVec) {
        std::wcout << fileData.EncryptedFilePath << std::endl;
      }

      std::cout << "--Decrypting Files--" << std::endl;
      std::vector<bool> decryptResults(fileDataVec.size(), false);
      dllUtils.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), decryptResults);

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
        if (dllUtils.DeleteStructFromJson(&fileData, globalDefinitions::jsonFileName)) {
          std::wcout << L"++Deleted struct for file: " << fileData.OriginalFilePath << "++" << std::endl;
        }
      }
    }
  }

  return 0;
}