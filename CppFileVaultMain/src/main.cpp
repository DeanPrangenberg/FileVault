#include "StructUtils/StructUtils.h"
#include "DLLUtils/DLLUtils.h"
#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

int main() {
  auto dllUtils = DLLUtils();

  std::cout << "--Starting file scan--" << std::endl;
  auto pathList = dllUtils.ScanDirectory("S:\\", false);
  std::cout << "Scan completed found " << pathList.size() << " files!" << std::endl;

  std::vector<FileData> fileDataVec;
  if (!pathList.empty()) {
    std::cout << "--Building Structs--" << std::endl;
    for (const auto &filePath: pathList) {
      auto filePathstr = filePath.wstring();

      if (filePathstr.empty() || filePathstr.find('\\') == std::string::npos) {
        continue;
      }

      std::wcout << L"Generating Struct for path: " << filePathstr << std::endl;
      fileDataVec.push_back(
          StructUtils::createFileDataStruct(globalDefinitions::AlgorithmType::AES256, fs::path(filePath)));
    }

    if (!fileDataVec.empty()) {
      std::cout << "--Encrypting Files--" << std::endl;
      std::vector<bool> encryptResults;
      dllUtils.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), encryptResults);

      for (int i = 0; i < encryptResults.size(); i++) {
        if (!encryptResults[i]) {
          fileDataVec.erase(fileDataVec.begin() + i);
        }
      }

      std::cout << "--" << fileDataVec.size() << " Files Encrypted--" << std::endl;

      std::cout << "--Saving File Data--" << std::endl;
      dllUtils.WriteFileDataToJson(fileDataVec);

      // Clear the current fileDataVec and rescan the directory
      fileDataVec.clear();
      pathList = dllUtils.ScanDirectory("S:\\clips\\cut", true);

      // Reload the FileData structs from the JSON file
      std::cout << "--Reloading File Data--" << std::endl;
      for (const auto &filePath: pathList) {
        auto *partialStruct = new FileData();
        std::wstring filePathStr = filePath.wstring();
        partialStruct->EncryptedFilePath = StructUtils::ConvertWStringToWChar(filePathStr);
        if (dllUtils.FindAndCompleteStruct(partialStruct, globalDefinitions::jsonFileName)) {
          FileData fileData = *partialStruct;
          delete partialStruct;
          fileDataVec.push_back(fileData);
          std::wcout << L"Found and completed struct for file: " << fileData.OriginalFilePath << std::endl;
        }
      }

      std::cout << "--Decrypting Files--" << std::endl;
      std::vector<bool> decryptResults;
      dllUtils.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()), decryptResults);

      for (int i = 0; i < decryptResults.size(); i++) {
        if (!decryptResults[i]) {
          fileDataVec.erase(fileDataVec.begin() + i);
        }
      }

      std::cout << "--" << fileDataVec.size() << " Files Decrypted--" << std::endl;

      for (const auto &fileData: fileDataVec) {
        if (dllUtils.DeleteStructFromJson(&fileData, globalDefinitions::jsonFileName)) {
          std::wcout << L"Deleted struct for file: " << fileData.OriginalFilePath << std::endl;
        }
      }

    }
  }

  return 0;
}