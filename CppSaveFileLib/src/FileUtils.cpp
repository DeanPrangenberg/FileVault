#include "SaveFileOperations.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

std::vector<FileData> SaveFileLib::readFileDataFromJson(const wchar_t *filePath) {
  std::vector<FileData> fileDataList;
  std::ifstream file(filePath);
  if (file.is_open()) {
    json j;
    file >> j;
    for (const auto &item : j) {
      fileDataList.push_back(deserializeJsonToFileData(item));
    }
    file.close();
  } else {
    std::wcerr << L"Failed to open file for reading: " << filePath << std::endl;
  }
  return fileDataList;
}

void SaveFileLib::createFileIfNotExists(const wchar_t *filePath) {
  if (printDebug) std::wcout << "Checking file path: " << filePath << std::endl;
  std::ifstream infile(filePath);
  if (!infile.good()) {
    std::ofstream outfile(filePath);
    outfile.close();
  }
}