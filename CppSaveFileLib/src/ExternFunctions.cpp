//
// Created by prang on 27.10.2024.
//

#include "SaveFileOperations.h"
#include <iostream>
#include <fstream>

extern "C" {
void WriteFileDataToJson(const FileData *fileDataList, size_t fileDataSize, const wchar_t *filePath) {
  if (printDebug) std::cout << "Checking file" << std::endl;
  SaveFileLib::createFileIfNotExists(filePath);
  if (printDebug) std::cout << "Checking done" << std::endl;

  json j;
  if (printDebug) std::cout << "Building Json Data" << std::endl;
  for (int i = 0; i < fileDataSize; ++i) {
    const auto &fileData = fileDataList[i];
    SaveFileLib lib;
    json serializedData = lib.serializeFileDataToJson(fileData);
    if (printDebug) std::cout << serializedData.dump(4) << std::endl;
    j.push_back(serializedData);
  }

  std::ofstream file(filePath);
  if (file.is_open()) {
    file << j.dump(4);
    file.close();
  } else {
    std::wcerr << L"Failed to open file for writing: " << filePath << std::endl;
  }
}

bool FindAndCompleteStruct(FileData *partialStruct, const wchar_t *filePath) {
  SaveFileLib lib;
  auto fileDataList = lib.readFileDataFromJson(filePath);
  auto match = lib.findMatchingFileData(partialStruct, fileDataList);
  if (match) {
    *partialStruct = *match;
    return true;
  }
  return false;
}

bool DeleteStructFromJson(const FileData *targetStruct, const wchar_t *filePath) {
  SaveFileLib lib;
  auto fileDataList = lib.readFileDataFromJson(filePath);
  auto it = std::remove_if(fileDataList.begin(), fileDataList.end(), [&](const FileData& data) {
    return lib.compareFileData(targetStruct, data);
  });
  if (it != fileDataList.end()) {
    fileDataList.erase(it, fileDataList.end());
    WriteFileDataToJson(fileDataList.data(), fileDataList.size(), filePath);
    return true;
  }
  return false;
}
}