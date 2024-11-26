#include <array>
#include <iostream>
#include <random>
#include "CryptoAPI.h"
#include "../alogs/AES128.h"
#include "../alogs/AES256.h"
#include "../alogs/KeyGen.h"
#include "../alogs/SHA512.h"

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
  if (key == nullptr || iv == nullptr) {
    std::cerr << "Error: key or iv pointer is null." << std::endl;
    return;
  }

  std::vector<unsigned char> keyVec(keySize);
  std::vector<unsigned char> ivVec(16); // 128-bit IV

  KeyGen::generateKeyIv(keySize, keyVec, ivVec);

  std::copy(keyVec.begin(), keyVec.end(), key);
  std::copy(ivVec.begin(), ivVec.end(), iv);
}


[[maybe_unused]] CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char *fileID) {
  auto hash = SHA512::hashFile(filePath);
  auto hashStr = SHA512::hashString(std::wstring(filePath));

  std::array<unsigned char, EVP_MAX_MD_SIZE> hashID;

  // combine both hashes with XOR and a random value
  for (int i = 0; i < 64; i++) {
    // Generate a random value
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    unsigned char randomValue = static_cast<unsigned char>(dis(gen));
    if (i % 2 == 0) {
      hashID[i] = hash[i] ^ randomValue;
    } else {
      hashID[i] = hashStr[i] ^ randomValue;
    }
  }

  std::copy(hashID.begin(), hashID.begin() + 64, fileID);
}
}
