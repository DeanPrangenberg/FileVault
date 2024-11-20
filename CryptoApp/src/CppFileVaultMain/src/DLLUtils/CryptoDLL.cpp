//
// Created by prang on 20.11.2024.
//

#include "CryptoDLL.h"


void CryptoDLL::GenerateKeyIv(size_t size, unsigned char *key, unsigned char *iv) {
  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto generateKeyIvFunc = (GenerateKeyIvFunc) GetProcAddress(hCryptographyDll, "GenerateKeyIv");
  if (!generateKeyIvFunc) {
    logError("Failed to get function address for GenerateKeyIv");
    unloadDll(hCryptographyDll);
    return;
  }

  generateKeyIvFunc(size, key, iv);
  unloadDll(hCryptographyDll);
}

void CryptoDLL::GenerateFileID(const wchar_t *filePath, unsigned char *fileID) {
  if (!filePath || !fileID) {
    logError("Invalid file path or fileID buffer");
    return;
  }

  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto generateFileIDFunc = (GenerateFileIDFunc) GetProcAddress(hCryptographyDll, "GenerateFileID");
  if (!generateFileIDFunc) {
    logError("Failed to get function address for GenerateFileID");
    unloadDll(hCryptographyDll);
    return;
  }

  generateFileIDFunc(filePath, fileID);
  unloadDll(hCryptographyDll);
}