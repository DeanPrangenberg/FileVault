#include "StructUtils.h"
#include "../DLLUtils/DLLUtils.h"
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
  auto dllUtils = DLLUtils();

  unsigned char iv[16];
  unsigned char *key = nullptr;
  int keyLength = 0;

  if (algorithmenType == AlgorithmType::AES128) {
    key = new unsigned char[16];
    keyLength = 16;
    dllUtils.GenerateKeyIv(keyLength, key, iv);
  } else if (algorithmenType == AlgorithmType::AES256) {
    key = new unsigned char[32];
    keyLength = 32;
    dllUtils.GenerateKeyIv(keyLength, key, iv);
  }

  unsigned char *fileID = nullptr;
  int fileIDLength = 0;
  dllUtils.GenerateFileID(originalFilePath.wstring().c_str(), &fileID, &fileIDLength);

  FileData fileData;

  std::wstring encryptedFilePath = originalFilePath.wstring() + globalDefinitions::encFileSuffix;
  std::wstring algorithmenTypeStr = AlgorithmTypeToString(algorithmenType);

  fileData.FileID = new unsigned char[fileIDLength];
  std::memcpy(fileData.FileID, fileID, fileIDLength);
  fileData.fileIDLength = fileIDLength;
  fileData.AlgorithmenType = ConvertWStringToWChar(algorithmenTypeStr);
  fileData.OriginalFilePath = ConvertWStringToWChar(originalFilePath.wstring());
  fileData.EncryptedFilePath = ConvertWStringToWChar(encryptedFilePath);
  fileData.DecryptedFilePath = ConvertWStringToWChar(originalFilePath.wstring());
  fileData.Key = new unsigned char[keyLength];
  std::memcpy(fileData.Key, key, keyLength);
  fileData.keyLength = keyLength;
  fileData.Iv = new unsigned char[16];
  std::memcpy(fileData.Iv, iv, 16);
  fileData.ivLength = 16;

  delete[] key;
  delete[] fileID;

  return fileData;
}