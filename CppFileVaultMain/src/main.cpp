#include "StructUtils/StructUtils.h"
#include "DLLUtils/DLLUtils.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
  auto dllUtils = DLLUtils();

  std::cout << "Creating paths" << std::endl;
  auto pathList = dllUtils.ScanDirectory("S:\\", false);

  std::vector<FileData> fileDataVec;

  for (const auto &filePath : pathList) {
    auto filePathstr = filePath.wstring();
    if (filePathstr.empty()) {
      continue;
    }
    if (filePathstr.find('\\') != std::string::npos) {
      std::wcout << L"Building Struct for path: " << filePathstr << std::endl;
      fileDataVec.push_back(StructUtils::createFileDataStruct(globalDefinitions::AlgorithmType::AES256, fs::path(filePath)));
    }
  }

  if (!fileDataVec.empty()) {
    std::cout << "En/Decrypting Files" << std::endl;
    dllUtils.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()));
    dllUtils.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()));

    std::cout << "Saving File Data" << std::endl;
    dllUtils.WriteFileDataToJson(fileDataVec);
  }

  return 0;
}