//
// Created by prang on 20.11.2024.
//

#include "MultiCryptoDLL.h"

void MultiCryptoDLL::EncryptFiles(struct FileData *fileData, int count, std::vector<bool> &results) {
  HMODULE hMultiThreadCryptoLib = loadDll(L"GoMultiThreadCryptoLib.dll");
  if (!hMultiThreadCryptoLib) {
    return;
  }

  auto encryptFilesFunc = (EncryptFilesFunc) GetProcAddress(hMultiThreadCryptoLib, "EncryptFilesWrapper");
  if (!encryptFilesFunc) {
    logError("Failed to get function address for EncryptFilesWrapper");
    unloadDll(hMultiThreadCryptoLib);
    return;
  }

  bool *resultArray = encryptFilesFunc(fileData, count);
  results.assign(resultArray, resultArray + count);

  unloadDll(hMultiThreadCryptoLib);
}

void MultiCryptoDLL::DecryptFiles(struct FileData *fileData, int count, std::vector<bool> &results) {
  HMODULE hMultiThreadCryptoLib = loadDll(L"GoMultiThreadCryptoLib.dll");
  if (!hMultiThreadCryptoLib) {
    return;
  }

  auto decryptFilesFunc = (DecryptFilesFunc) GetProcAddress(hMultiThreadCryptoLib, "DecryptFilesWrapper");
  if (!decryptFilesFunc) {
    logError("Failed to get function address for DecryptFilesWrapper");
    unloadDll(hMultiThreadCryptoLib);
    return;
  }

  bool *resultArray = decryptFilesFunc(fileData, count);
  results.assign(resultArray, resultArray + count);

  unloadDll(hMultiThreadCryptoLib);
}