//
// Created by prang on 20.11.2024.
//

#include "FileMarkDLL.h"

bool FileMarkDLL::ExtractFileIDFromFile(const wchar_t * filePath, unsigned char FileID[64]) {
  HMODULE hFileMarkDll = loadDll(L"CppFileMarkLib.dll");
  if (!hFileMarkDll) {
    return false;
  }

  auto extractFileIDFromFileFunc = (ExtractFileIDFromFileFunc) GetProcAddress(hFileMarkDll, "extractFileIDFromFile");
  if (!extractFileIDFromFileFunc) {
    logError("Failed to get function address for extractFileIDFromFile");
    unloadDll(hFileMarkDll);
    return false;
  }

  bool result = extractFileIDFromFileFunc(filePath, FileID);
  unloadDll(hFileMarkDll);
  return result;
}