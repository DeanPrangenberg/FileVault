#include <array>
#include <iostream>
#include "CryptoNamespace.h"

// Wrapper functions for C compatibility
extern "C" {
[[maybe_unused]] CRYPTOLIB_API bool EncryptFileWrapper(const struct FileData *fileData) {
  if (fileData->keyLength == 16) { // AES-128
    return AES128::encryptFile(fileData);
  } else if (fileData->keyLength == 32) { // AES-256
    return AES256::encryptFile(fileData);
  }
  return false;
}

[[maybe_unused]] CRYPTOLIB_API bool DecryptFileWrapper(const struct FileData *fileData) {
  if (fileData->keyLength == 16) { // AES-128
    return AES128::decryptFile(fileData);
  } else if (fileData->keyLength == 32) { // AES-256
    return AES256::decryptFile(fileData);
  }
  return false;
}

[[maybe_unused]] CRYPTOLIB_API void GenerateKeyIv(size_t keySize, unsigned char *key, unsigned char *iv) {
  std::vector<unsigned char> keyVec(key, key + keySize);
  std::vector<unsigned char> ivVec(iv, iv + 16); // 128-bit
  KeyGen::generateKeyIv(keySize, keyVec, ivVec);
}

[[maybe_unused]] CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char *fileID) {
  auto hash = SHA512::hashFile(filePath);
  std::copy(hash.begin(), hash.begin() + 64, fileID);
}
}
