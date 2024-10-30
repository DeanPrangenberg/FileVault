//
// Created by prang on 27.10.2024.
//

#include "SaveFileOperations.h"
#include <iostream>
#include <fstream>

extern "C" {
[[maybe_unused]] void WriteFileDataToJson(const FileData *fileDataList, size_t fileDataSize, const wchar_t *saveFilePath) {
  SaveFileLib::createFileIfNotExists(saveFilePath);

  // Read existing data from the file
  std::vector<FileData> existingFileDataList;
  std::ifstream infile(saveFilePath);
  if (infile.peek() != std::ifstream::traits_type::eof()) {
    existingFileDataList = SaveFileLib().readFileDataFromJson(saveFilePath);
  }
  infile.close();

  // Append new data to the existing data
  for (size_t i = 0; i < fileDataSize; ++i) {
    existingFileDataList.push_back(fileDataList[i]);
  }

  // Serialize the combined data to JSON
  json j;
  for (const auto &fileData : existingFileDataList) {
    SaveFileLib lib;
    json serializedData = lib.serializeFileDataToJson(fileData);
    j.push_back(serializedData);
  }

  // Write the combined data back to the file
  std::ofstream file(saveFilePath);
  if (file.is_open()) {
    file << j.dump(4);
    file.close();
  } else {
    std::wcerr << L"Failed to open file for writing: " << saveFilePath << std::endl;
  }
}

[[maybe_unused]] bool FindAndCompleteStruct(FileData *partialStruct, const wchar_t *saveFilePath) {
  SaveFileLib lib;
  auto fileDataList = lib.readFileDataFromJson(saveFilePath);
  auto match = lib.findMatchingFileData(partialStruct, fileDataList);
  if (match) {
    *partialStruct = *match;
    return true;
  }
  return false;
}

[[maybe_unused]] bool DeleteStructFromJson(const FileData *targetStruct, const wchar_t *saveFilePath) {
  SaveFileLib lib;
  auto fileDataList = lib.readFileDataFromJson(saveFilePath);
  auto it = std::remove_if(fileDataList.begin(), fileDataList.end(), [&](const FileData& data) {
    return lib.compareFileData(targetStruct, data);
  });
  if (it != fileDataList.end()) {
    fileDataList.erase(it, fileDataList.end());
    WriteFileDataToJson(fileDataList.data(), fileDataList.size(), saveFilePath);
    return true;
  }
  return false;
}
}