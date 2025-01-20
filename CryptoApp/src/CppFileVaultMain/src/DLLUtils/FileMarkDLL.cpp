//
// Created by prang on 20.11.2024.
//

#include "FileMarkDLL.h"

/**
 * @brief Extracts FileID and EncryptionID from a file using the CppFileMarkLib DLL.
 *
 * @param filePath The path of the file to extract IDs from.
 * @param FileID Buffer to store the extracted FileID.
 * @param EncryptionID Buffer to store the extracted EncryptionID.
 * @return True if the extraction was successful, false otherwise.
 */
bool FileMarkDLL::extractIDsFromFile(const fs::path *filePath, std::vector<unsigned char> *FileID,
                                     std::vector<unsigned char> *EncryptionID) {
  HMODULE hFileMarkDll = loadDll(L"CppFileMarkLib.dll");
  if (!hFileMarkDll) {
    return false;
  }

  auto extractIDsFromFileFunc = (ExtractIDsFromFileFunc) GetProcAddress(hFileMarkDll, "extractIDsFromFile");
  if (!extractIDsFromFileFunc) {
    Logs::writeToErrorLog("Failed to get function address for extractIDsFromFile");
    unloadDll(hFileMarkDll);
    return false;
  }

  bool result = extractIDsFromFileFunc(filePath, FileID, EncryptionID);
  unloadDll(hFileMarkDll);
  return result;
}