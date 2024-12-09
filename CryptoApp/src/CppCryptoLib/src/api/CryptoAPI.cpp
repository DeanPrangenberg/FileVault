#include "CryptoAPI.h"

extern "C" {
[[maybe_unused]] CRYPTOLIB_API bool *EncryptFileWrapper(const FileData *fileData, const int numFiles) {
  std::vector<bool> results;
  for (int i = 0; i < numFiles; i++) {
    const FileData &file = fileData[i];
    if (file.getKeyLength() == 16) { // AES-128
      results.push_back(AES128::encryptFile(&file));
    } else if (file.getKeyLength() == 32) { // AES-256
      results.push_back(AES256::encryptFile(&file));
    }
  }
  bool *resultArray = new bool[results.size()];
  std::copy(results.begin(), results.end(), resultArray);
  return resultArray;
}

[[maybe_unused]] CRYPTOLIB_API bool *DecryptFileWrapper(const FileData *fileData, const int numFiles) {
  std::vector<bool> results;
  for (int i = 0; i < numFiles; i++) {
    const FileData &file = fileData[i];
    if (file.getKeyLength() == 16) { // AES-128
      results.push_back(AES128::decryptFile(&file));
    } else if (file.getKeyLength() == 32) { // AES-256
      results.push_back(AES256::decryptFile(&file));
    }
  }
  bool *resultArray = new bool[results.size()];
  std::copy(results.begin(), results.end(), resultArray);
  return resultArray;
}

[[maybe_unused]] CRYPTOLIB_API void GenerateKeyIv(size_t keySize, unsigned char *key, unsigned char *iv) {
  KeyGen keyGen;

  if (key == nullptr || iv == nullptr) {
    std::cerr << "Error: key or iv pointer is null." << std::endl;
    return;
  }

  std::vector<unsigned char> keyVec(keySize);
  std::vector<unsigned char> ivVec(16); // 128-bit IV


  keyGen.generateKeyIv(keySize, keyVec, ivVec);

  std::copy(keyVec.begin(), keyVec.end(), key);
  std::copy(ivVec.begin(), ivVec.end(), iv);
}


[[maybe_unused]] CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char *fileID) {
  auto hash = SHA512::hashFile(filePath);
  auto hashStr = SHA512::hashString(std::wstring(filePath));

  std::vector<unsigned char> combinedHash;
  combinedHash.reserve(hash.size() + hashStr.size());
  combinedHash.insert(combinedHash.end(), hash.begin(), hash.end());
  combinedHash.insert(combinedHash.end(), hashStr.begin(), hashStr.end());

  std::array<unsigned char, EVP_MAX_MD_SIZE> hashID = SHA512::hashArray(combinedHash);

  std::copy(hashID.begin(), hashID.begin() + 64, fileID);
}
}
