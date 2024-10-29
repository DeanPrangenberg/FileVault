//
// Created by prang on 27.10.2024.
//

#include "SaveFileOperations.h"
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

std::string SaveFileLib::toBinaryString(const unsigned char *data, int length) {
  std::ostringstream oss;
  for (int i = 0; i < length; ++i) {
    for (int bit = 7; bit >= 0; --bit) {
      oss << ((data[i] >> bit) & 1);
    }
  }
  return oss.str();
}

std::string SaveFileLib::wstringToUtf8(const wchar_t *wstr) {
  if (!wstr || wcslen(wstr) == 0) {
    if (printDebug) std::cout << "Empty wchar_t*" << std::endl;
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

std::vector<unsigned char> SaveFileLib::fromBinaryString(const std::string &binaryString) {
  std::vector<unsigned char> data(binaryString.size() / 8);
  for (size_t i = 0; i < data.size(); ++i) {
    for (int bit = 7; bit >= 0; --bit) {
      if (binaryString[i * 8 + (7 - bit)] == '1') {
        data[i] |= (1 << bit);
      }
    }
  }
  return data;
}

// Function to convert UTF-8 string to wchar_t*
std::wstring SaveFileLib::utf8ToWstring(const std::string &str) {
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
  std::wstring wstrTo(size_needed, 0);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
  return wstrTo;
}

FileData SaveFileLib::deserializeJsonToFileData(const json &j) {
  FileData fileData;
  auto fileID = fromBinaryString(j["FileID"]);
  fileData.FileID = new unsigned char[fileID.size()];
  std::copy(fileID.begin(), fileID.end(), fileData.FileID);
  fileData.fileIDLength = j["fileIDLength"];
  fileData.AlgorithmenType = _wcsdup(utf8ToWstring(j["AlgorithmenType"]).c_str());
  fileData.OriginalFilePath = _wcsdup(utf8ToWstring(j["OriginalFilePath"]).c_str());
  fileData.EncryptedFilePath = _wcsdup(utf8ToWstring(j["EncryptedFilePath"]).c_str());
  fileData.DecryptedFilePath = _wcsdup(utf8ToWstring(j["DecryptedFilePath"]).c_str());
  auto key = fromBinaryString(j["Key"]);
  fileData.Key = new unsigned char[key.size()];
  std::copy(key.begin(), key.end(), fileData.Key);
  fileData.keyLength = j["keyLength"];
  auto iv = fromBinaryString(j["Iv"]);
  fileData.Iv = new unsigned char[iv.size()];
  std::copy(iv.begin(), iv.end(), fileData.Iv);
  fileData.ivLength = j["ivLength"];
  return fileData;
}

json SaveFileLib::serializeFileDataToJson(const FileData &fileData) {
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