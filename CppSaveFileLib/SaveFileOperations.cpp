#include "SaveFileOperations.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <windows.h>

using json = nlohmann::json;

std::string toBinaryString(const unsigned char *data, int length) {
  std::ostringstream oss;
  for (int i = 0; i < length; ++i) {
    for (int bit = 7; bit >= 0; --bit) {
      oss << ((data[i] >> bit) & 1);
    }
  }
  return oss.str();
}

std::string wstringToUtf8(const wchar_t *wstr) {
  if (!wstr || wcslen(wstr) == 0) {
    std::cout << "Empty wchar_t*" << std::endl;
    return std::string();
  }
  int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, (int) wcslen(wstr), NULL, 0, NULL, NULL);
  if (size_needed <= 0) {
    std::cerr << "WideCharToMultiByte failed to calculate size_needed" << std::endl;
    return std::string();
  }
  std::string strTo(size_needed, 0);
  int result = WideCharToMultiByte(CP_UTF8, 0, wstr, (int) wcslen(wstr), &strTo[0], size_needed, NULL, NULL);
  if (result <= 0) {
    std::cerr << "WideCharToMultiByte failed to convert wchar_t* to UTF-8" << std::endl;
    return std::string();
  }
  return strTo;
}

json serializeFileDataToJson(const FileData &fileData) {
  json j;
  j["FileID"] = fileData.FileID ? toBinaryString(fileData.FileID, fileData.fileIDLength) : "";
  j["fileIDLength"] = fileData.fileIDLength;
  j["AlgorithmenType"] = fileData.AlgorithmenType ? wstringToUtf8(fileData.AlgorithmenType) : "";
  j["OriginalFilePath"] = wstringToUtf8(fileData.OriginalFilePath);
  j["EncryptedFilePath"] = wstringToUtf8(fileData.EncryptedFilePath);
  j["DecryptedFilePath"] = wstringToUtf8(fileData.DecryptedFilePath);
  j["Key"] = toBinaryString(fileData.Key, fileData.keyLength);
  j["keyLength"] = fileData.keyLength;
  j["Iv"] = toBinaryString(fileData.Iv, fileData.ivLength);
  j["ivLength"] = fileData.ivLength;
  return j;
}

void createFileIfNotExists(const wchar_t *filePath) {
  std::wcout << "Checking file path: " << filePath << std::endl;
  std::ifstream infile(filePath);
  if (!infile.good()) {
    std::ofstream outfile(filePath);
    outfile.close();
  }
}

void writeFileDataToJson(const FileData *fileDataList, size_t fileDataSize, const wchar_t *filePath) {
  std::cout << "Checking file" << std::endl;
  createFileIfNotExists(filePath);
  std::cout << "Checking done" << std::endl;

  json j;
  std::cout << "Building Json Data" << std::endl;
  for (int i = 0; i < fileDataSize; ++i) {
    const auto &fileData = fileDataList[i];
    json serializedData = serializeFileDataToJson(fileData);
    std::cout << serializedData.dump(4) << std::endl;
    j.push_back(serializeFileDataToJson(fileData));
  }

  std::ofstream file(filePath);
  if (file.is_open()) {
    file << j.dump(4);
    file.close();
  } else {
    std::wcerr << L"Failed to open file for writing: " << filePath << std::endl;
  }
}
