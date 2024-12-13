//
// Created by prang on 20.11.2024.
//

#include "FileMarkDLL.h"

bool FileMarkDLL::extractIDsFromFile(const wchar_t * filePath, unsigned char FileID[64], unsigned char EncryptionID[64]) {
  HMODULE hFileMarkDll = loadDll(L"CppFileMarkLib.dll");
  if (!hFileMarkDll) {
    return false;
  }

  auto extractIDsFromFileFunc = (ExtractIDsFromFileFunc) GetProcAddress(hFileMarkDll, "extractIDsFromFile");
  if (!extractIDsFromFileFunc) {
    logError("Failed to get function address for extractIDsFromFile");
    unloadDll(hFileMarkDll);
    return false;
  }

  bool result = extractIDsFromFileFunc(filePath, FileID, EncryptionID);
  unloadDll(hFileMarkDll);
  return result;
}