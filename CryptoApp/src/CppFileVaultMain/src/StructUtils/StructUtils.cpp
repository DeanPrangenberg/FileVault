#include "StructUtils.h"
#include "../DLLUtils/CryptoDLL.h"
#include <cstring>
#include <iostream>

std::wstring StructUtils::AlgorithmTypeToString(AlgorithmType type) {
  switch (type) {
    case AlgorithmType::AES128:
      return L"AES128";
    case AlgorithmType::AES256:
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

FileData StructUtils::createFileDataStruct(const AlgorithmType &algorithmenType, const fs::path &originalFilePath) {
  CryptoDLL cryptoDll;
  unsigned char IV[16];
  unsigned char *KEY = nullptr;
  int keyLength = 0;

  switch (algorithmenType) {
    case AlgorithmType::AES128:
      keyLength = 16;
      KEY = new unsigned char[keyLength];
      break;
    case AlgorithmType::AES256:
      keyLength = 32;
      KEY = new unsigned char[keyLength];
      break;
    default:
      throw std::invalid_argument("Unsupported algorithm type");
  }

  cryptoDll.GenerateKeyIv(keyLength, KEY, IV);

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

  fileData.setKey(new unsigned char[keyLength]);
  std::memcpy(fileData.getKey(), KEY, keyLength);
  fileData.setKeyLength(keyLength);

  fileData.setIv(new unsigned char[16]);
  std::memcpy(fileData.getIv(), IV, 16);
  fileData.setIvLength(16);

  fileData.debugFileData();

  return fileData;
}