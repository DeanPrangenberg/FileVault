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
  unsigned char iv[16];
  unsigned char *key = nullptr;
  int keyLength = 0;

  if (algorithmenType == AlgorithmType::AES128) {
    key = new unsigned char[16];
    keyLength = 16;
    cryptoDll.GenerateKeyIv(keyLength, key, iv);
  } else if (algorithmenType == AlgorithmType::AES256) {
    key = new unsigned char[32];
    keyLength = 32;
    cryptoDll.GenerateKeyIv(keyLength, key, iv);
  }

  int fileIDLength = 64; // SHA512 hash length
  unsigned char fileID[64];
  cryptoDll.GenerateFileID(originalFilePath.wstring().c_str(), fileID);

  // Converting data for struct
  std::wstring encryptedFilePath = originalFilePath.wstring() + globalDefinitions::encFileSuffix;
  std::wstring algorithmenTypeStr = AlgorithmTypeToString(algorithmenType);

  // Allocate memory for struct
  FileData fileData;
  fileData.FileID = new unsigned char[fileIDLength];
  fileData.fileIDLength = fileIDLength;
  fileData.AlgorithmenType = ConvertWStringToWChar(algorithmenTypeStr);
  fileData.OriginalFilePath = ConvertWStringToWChar(originalFilePath.wstring());
  fileData.EncryptedFilePath = ConvertWStringToWChar(encryptedFilePath);
  fileData.DecryptedFilePath = ConvertWStringToWChar(originalFilePath.wstring());
  fileData.Key = new unsigned char[keyLength];
  fileData.keyLength = keyLength;
  fileData.Iv = new unsigned char[16];
  fileData.ivLength = 16;

  // Copy data to struct
  std::memcpy(fileData.FileID, fileID, fileIDLength);
  std::memcpy(fileData.Key, key, keyLength);
  std::memcpy(fileData.Iv, iv, 16);

  delete[] key;

  return fileData;
}