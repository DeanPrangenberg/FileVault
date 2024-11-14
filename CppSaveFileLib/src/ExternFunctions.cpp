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
  json existingData = json::array(); // Initialize as an empty array
  std::ifstream inputFile(saveFilePath);
  if (inputFile.is_open()) {
    try {
      inputFile >> existingData;
    } catch (const std::exception &e) {
      // If the file is empty or contains invalid JSON, existingData remains an empty array
    }
    inputFile.close();
  }

  // Serialize the new data to JSON and append it to the existing data
  for (size_t i = 0; i < fileDataSize; ++i) {
    SaveFileLib lib;
    json serializedData = lib.serializeFileDataToJson(fileDataList[i]);
    existingData.push_back(serializedData);
  }

  // Write the combined data back to the file
  std::ofstream outputFile(saveFilePath);
  if (outputFile.is_open()) {
    outputFile << existingData.dump(4);
    outputFile.close();
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
  if (it != fileDataList.end() && it != fileDataList.begin()) {
    fileDataList.erase(it, fileDataList.end());
    try {
      // Overwrite the JSON file with the updated list
      std::ofstream outputFile(saveFilePath);
      if (outputFile.is_open()) {
        json updatedData = json::array();
        for (const auto& fileData : fileDataList) {
          updatedData.push_back(lib.serializeFileDataToJson(fileData));
        }
        outputFile << updatedData.dump(4);
        outputFile.close();
      } else {
        std::wcerr << L"Failed to open file for writing: " << saveFilePath << std::endl;
        return false;
      }
    } catch (const std::exception &e) {
      std::wcerr << L"Failed to write to JSON file: " << e.what() << std::endl;
      return false;
    }
    return true;
  }
  return false;
}
}