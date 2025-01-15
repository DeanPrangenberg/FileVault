#include "StructUtils.h"
#include "../DLLUtils/CryptoDLL.h"
#include <cstring>
#include <iostream>

std::wstring StructUtils::AlgorithmTypeToString(globalDefinitions::AlgorithmType type) {
  switch (type) {
    case globalDefinitions::AlgorithmType::AES128:
      return L"AES128";
    case globalDefinitions::AlgorithmType::AES256:
      return L"AES256";
    default:
      return L"Unknown";
  }
}

wchar_t *StructUtils::ConvertWStringToWChar(const std::wstring &input) {
  wchar_t *output = new wchar_t[input.size() + 1];
  std::wcscpy(output, input.c_str());
  return output;
}

FileData StructUtils::createFileDataStruct(const globalDefinitions::AlgorithmType &algorithmenType, const fs::path &originalFilePath) {
  CryptoDLL cryptoDll;
  unsigned char *IV;
  int ivLength = 0;
  unsigned char *KEY;
  int keyLength = 0;

  switch (algorithmenType) {
    case globalDefinitions::AlgorithmType::AES128:
      ivLength = 16;
      keyLength = 16;
      KEY = new unsigned char[keyLength];
      IV = new unsigned char[ivLength];
      break;
    case globalDefinitions::AlgorithmType::AES256:
      ivLength = 16;
      keyLength = 32;
      KEY = new unsigned char[keyLength];
      IV = new unsigned char[ivLength];
      break;
    default:
      throw std::invalid_argument("Unsupported algorithm type");
  }

  std::memset(KEY, 0, keyLength);
  std::memset(IV, 0, ivLength);

  cryptoDll.GenerateKeyIv(keyLength, ivLength, KEY, IV);

  unsigned char fileID[64];
  cryptoDll.GenerateFileID(originalFilePath.wstring().c_str(), fileID);

  unsigned char lastUpdateID[64];
  cryptoDll.getCurrentTimeHash(lastUpdateID);

  std::wstring encryptedFilePath = originalFilePath.wstring() + globalDefinitions::encFileSuffix;
  std::wstring algorithmenTypeStr = AlgorithmTypeToString(algorithmenType);

  FileData fileData;
  fileData.setFileId(new unsigned char[64]);
  std::memcpy(fileData.getFileId(), fileID, 64);
  fileData.setFileIdLength(64);

  fileData.setLastUpdateId(new unsigned char[64]);
  std::memcpy(fileData.getLastUpdateId(), lastUpdateID, 64);
  fileData.setLastUpdateIdLength(64);

  fileData.setAlgorithmenType(ConvertWStringToWChar(algorithmenTypeStr));
  fileData.setOriginalFilePath(ConvertWStringToWChar(originalFilePath.wstring()));
  fileData.setEncryptedFilePath(ConvertWStringToWChar(encryptedFilePath));
  fileData.setDecryptedFilePath(ConvertWStringToWChar(originalFilePath.wstring()));

  fileData.setKey(KEY);
  fileData.setKeyLength(keyLength);

  fileData.setIv(IV);
  fileData.setIvLength(ivLength);

  fileData.debugFileData();

  return fileData;
}