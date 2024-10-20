#include "StructUtils/StructUtils.h"
#include "DLLUtils/DLLUtils.h"
#include <filesystem>

namespace fs = std::filesystem;

int main() {
  auto dllUtils = DLLUtils();

  auto pathList = dllUtils.ProcessFileWithCsScanner("S:\\clips\\cut");

  std::vector<FileData> fileDataVec;

  for (const auto &filePath : pathList) {
    if (filePath.empty()) {
      continue;
    }
    if (filePath.find('/') != std::string::npos) {
      fileDataVec.push_back(StructUtils::createFileDataStruct(globalDefinitions::AlgorithmType::AES256, fs::path(filePath)));
    }
  }

  if (!fileDataVec.empty()) {
    dllUtils.EncryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()));
    dllUtils.DecryptFiles(fileDataVec.data(), static_cast<int>(fileDataVec.size()));

    dllUtils.WriteFileDataToJson(fileDataVec);
  }

  return 0;
}