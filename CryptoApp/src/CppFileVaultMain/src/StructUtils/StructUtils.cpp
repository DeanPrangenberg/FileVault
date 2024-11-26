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

  int fileIDLength = 64; // SHA512 hash length
  unsigned char fileID[64];
  cryptoDll.GenerateFileID(originalFilePath.wstring().c_str(), fileID);

  // Converting data for struct
  std::wstring encryptedFilePath = originalFilePath.wstring() + globalDefinitions::encFileSuffix;
  std::wstring algorithmenTypeStr = AlgorithmTypeToString(algorithmenType);

  // Allocate memory for struct
  FileData fileData;
  fileData.FileID = new unsigned char[fileIDLength];
  std::memcpy(fileData.FileID, fileID, fileIDLength);
  fileData.fileIDLength = fileIDLength;
  fileData.AlgorithmenType = ConvertWStringToWChar(algorithmenTypeStr);
  fileData.OriginalFilePath = ConvertWStringToWChar(originalFilePath.wstring());
  fileData.EncryptedFilePath = ConvertWStringToWChar(encryptedFilePath);
  fileData.DecryptedFilePath = ConvertWStringToWChar(originalFilePath.wstring());
  fileData.Key = new unsigned char[keyLength];
  std::memcpy(fileData.Key, KEY, keyLength);
  fileData.keyLength = keyLength;
  fileData.Iv = new unsigned char[16];
  std::memcpy(fileData.Iv, IV, 16);
  fileData.ivLength = 16;

  globalDefinitions::debugFileData(fileData);

  return fileData;
}