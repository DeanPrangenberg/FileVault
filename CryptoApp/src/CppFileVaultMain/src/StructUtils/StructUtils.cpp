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

  if (algorithmenType == AlgorithmType::AES128) {
    KEY = new unsigned char[16];
    keyLength = 16;
    cryptoDll.GenerateKeyIv(keyLength, KEY, IV);
  } else if (algorithmenType == AlgorithmType::AES256) {
    KEY = new unsigned char[32];
    keyLength = 32;
    cryptoDll.GenerateKeyIv(keyLength, KEY, IV);
  }

  unsigned char fileID[64];
  cryptoDll.GenerateFileID(originalFilePath.wstring().c_str(), fileID);

  // Converting data for struct
  std::wstring encryptedFilePath = originalFilePath.wstring() + globalDefinitions::encFileSuffix;
  std::wstring algorithmenTypeStr = AlgorithmTypeToString(algorithmenType);

  // Allocate memory for struct
  FileData fileData;
  fileData.setFileId(new unsigned char[64]);
  std::memcpy(fileData.getFileId(), fileID, 64);
  fileData.setFileIdLength(64);
  fileData.setAlgorithmenType(ConvertWStringToWChar(algorithmenTypeStr));
  fileData.setOriginalFilePath(ConvertWStringToWChar(originalFilePath.wstring()));
  fileData.setEncryptedFilePath(ConvertWStringToWChar(encryptedFilePath));
  fileData.setDecryptedFilePath(ConvertWStringToWChar(originalFilePath.wstring()));
  fileData.setKey(new unsigned char[64]);
  std::memcpy(fileData.getKey(), KEY, keyLength);
  fileData.setKeyLength(keyLength);
  fileData.setIv(new unsigned char[16]);
  std::memcpy(fileData.getIv(), IV, 16);
  fileData.setIvLength(16);

  fileData.debugFileData();

  return fileData;
}