//
// Created by prang on 20.11.2024.
//

#include "CryptoDLL.h"

/**
 * @brief Encrypts multiple files using the CppCryptoLib DLL.
 *
 * @param fileData Pointer to an array of FileData objects representing the files to encrypt.
 * @param count The number of files to encrypt.
 * @param results A vector to store the results of the encryption for each file.
 */
void CryptoDLL::EncryptFiles(FileData *fileData, int count, std::vector<bool> &results) {
  HMODULE hMultiThreadCryptoLib = loadDll(L"CppCryptoLib.dll");
  if (!hMultiThreadCryptoLib) {
    return;
  }

  auto encryptFilesFunc = (EncryptFilesFunc) GetProcAddress(hMultiThreadCryptoLib, "EncryptFileWrapper");
  if (!encryptFilesFunc) {
    Logs::writeToErrorLog("Failed to get function address for EncryptFilesWrapper");
    unloadDll(hMultiThreadCryptoLib);
    return;
  }

  bool *resultArray = encryptFilesFunc(fileData, count);
  results.assign(resultArray, resultArray + count);

  delete[] resultArray;
  resultArray = nullptr;

  unloadDll(hMultiThreadCryptoLib);
}

/**
 * @brief Decrypts multiple files using the CppCryptoLib DLL.
 *
 * @param fileData Pointer to an array of FileData objects representing the files to decrypt.
 * @param count The number of files to decrypt.
 * @param results A vector to store the results of the decryption for each file.
 */
void CryptoDLL::DecryptFiles(FileData *fileData, int count, std::vector<bool> &results) {
  HMODULE hMultiThreadCryptoLib = loadDll(L"CppCryptoLib.dll");
  if (!hMultiThreadCryptoLib) {
    return;
  }

  auto decryptFilesFunc = (DecryptFilesFunc) GetProcAddress(hMultiThreadCryptoLib, "DecryptFileWrapper");
  if (!decryptFilesFunc) {
    Logs::writeToErrorLog("Failed to get function address for DecryptFilesWrapper");
    unloadDll(hMultiThreadCryptoLib);
    return;
  }

  bool *resultArray = decryptFilesFunc(fileData, count);
  results.assign(resultArray, resultArray + count);

  delete[] resultArray;
  resultArray = nullptr;

  unloadDll(hMultiThreadCryptoLib);
}

/**
 * @brief Generates a key and initialization vector (IV) using the CppCryptoLib DLL.
 *
 * @param keySize The size of the key to generate.
 * @param ivSize The size of the IV to generate.
 * @param key Pointer to the buffer to store the generated key.
 * @param iv Pointer to the buffer to store the generated IV.
 */
void CryptoDLL::GenerateKeyIv(size_t keySize, size_t ivSize, unsigned char *key, unsigned char *iv) {
  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto generateKeyIvFunc = (GenerateKeyIvFunc) GetProcAddress(hCryptographyDll, "GenerateKeyIv");
  if (!generateKeyIvFunc) {
    Logs::writeToErrorLog("Failed to get function address for GenerateKeyIv");
    unloadDll(hCryptographyDll);
    return;
  }

  generateKeyIvFunc(keySize, ivSize, key, iv);
  unloadDll(hCryptographyDll);
}

/**
 * @brief Generates a file ID for a given file path using the CppCryptoLib DLL.
 *
 * @param filePath The path of the file to generate the ID for.
 * @param fileID Pointer to the buffer to store the generated file ID.
 */
void CryptoDLL::GenerateFileID(const wchar_t *filePath, unsigned char *fileID) {
  if (!filePath || !fileID) {
    Logs::writeToErrorLog("Invalid file path or fileID buffer");
    return;
  }

  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto generateFileIDFunc = (GenerateFileIDFunc) GetProcAddress(hCryptographyDll, "GenerateFileID");
  if (!generateFileIDFunc) {
    Logs::writeToErrorLog("Failed to get function address for GenerateFileID");
    unloadDll(hCryptographyDll);
    return;
  }

  generateFileIDFunc(filePath, fileID);
  unloadDll(hCryptographyDll);
}

/**
 * @brief Generates a hash of the current time using the CppCryptoLib DLL.
 *
 * @param timeHash Pointer to the buffer to store the generated time hash.
 */
void CryptoDLL::getCurrentTimeHash(unsigned char *timeHash) {
  if (!timeHash) {
    Logs::writeToErrorLog("Invalid timeHash buffer");
    return;
  }

  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto getCurrentTimeHash = (getCurrentTimeHashFunc) GetProcAddress(hCryptographyDll, "getCurrentTimeHash");
  if (!getCurrentTimeHash) {
    Logs::writeToErrorLog("Failed to get function address for GenerateFileID");
    unloadDll(hCryptographyDll);
    return;
  }

  getCurrentTimeHash(timeHash);

  unloadDll(hCryptographyDll);
}

void CryptoDLL::GeneratePasswordHash(unsigned char *password, unsigned char *salt, unsigned char *hash,
                                     size_t passwordLength, size_t saltLength, size_t hashSize) {
  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto generatePasswordHashFunc = (GeneratePasswordHashFunc) GetProcAddress(hCryptographyDll, "GeneratePasswordHash");
  if (!generatePasswordHashFunc) {
    Logs::writeToErrorLog("Failed to get function address for GeneratePasswordHash");
    unloadDll(hCryptographyDll);
    return;
  }

  generatePasswordHashFunc(password, salt, hash, passwordLength, saltLength, hashSize);
  unloadDll(hCryptographyDll);
}

void CryptoDLL::generateRandomBytes(size_t randomBytesSize, unsigned char *randomBytes) {
  HMODULE hCryptographyDll = loadDll(L"CppCryptoLib.dll");
  if (!hCryptographyDll) {
    return;
  }

  auto generateRandomBytesFunc = (GenerateRandomBytesFunc) GetProcAddress(hCryptographyDll, "generateRandomBytes");
  if (!generateRandomBytesFunc) {
    Logs::writeToErrorLog("Failed to get function address for generateRandomBytes");
    unloadDll(hCryptographyDll);
    return;
  }

  generateRandomBytesFunc(randomBytesSize, randomBytes);
  unloadDll(hCryptographyDll);
}
