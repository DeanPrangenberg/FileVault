//
// Created by prang on 20.11.2024.
//

#include "CryptoDLL.h"

void CryptoDLL::EncryptFiles(struct FileData *fileData, int count, std::vector<bool> &results) {
  HMODULE hMultiThreadCryptoLib = loadDll(L"CppCryptoLib.dll");
  if (!hMultiThreadCryptoLib) {
    return;
  }

  auto encryptFilesFunc = (EncryptFilesFunc) GetProcAddress(hMultiThreadCryptoLib, "EncryptFileWrapper");
  if (!encryptFilesFunc) {
    logError("Failed to get function address for EncryptFilesWrapper");
    unloadDll(hMultiThreadCryptoLib);
    return;
  }

  bool *resultArray = encryptFilesFunc(fileData, count);
  results.assign(resultArray, resultArray + count);

  delete[]  resultArray;
  resultArray = nullptr;

  unloadDll(hMultiThreadCryptoLib);
}

void CryptoDLL::DecryptFiles(struct FileData *fileData, int count, std::vector<bool> &results) {
  HMODULE hMultiThreadCryptoLib = loadDll(L"CppCryptoLib.dll");
  if (!hMultiThreadCryptoLib) {
    return;
  }

  auto decryptFilesFunc = (DecryptFilesFunc) GetProcAddress(hMultiThreadCryptoLib, "DecryptFileWrapper");
  if (!decryptFilesFunc) {
    logError("Failed to get function address for DecryptFilesWrapper");
    unloadDll(hMultiThreadCryptoLib);
    return;
  }

  bool *resultArray = decryptFilesFunc(fileData, count);
  results.assign(resultArray, resultArray + count);

  delete[]  resultArray;
  resultArray = nullptr;

  unloadDll(hMultiThreadCryptoLib);
}

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