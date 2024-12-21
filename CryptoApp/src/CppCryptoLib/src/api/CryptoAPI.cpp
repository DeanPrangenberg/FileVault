#include "CryptoAPI.h"
#include <thread>
#include <future>
#include <vector>

extern "C" {
[[maybe_unused]] CRYPTOLIB_API bool *EncryptFileWrapper(FileData *fileData, const int numFiles) {
  std::vector<std::future<bool>> futures;
  for (int i = 0; i < numFiles; i++) {
    fileData[i].setEncryptionId(new unsigned char[64]);
    fileData[i].setEncryptionIdLength(64);

    getCurrentTimeHash(fileData[i].getEncryptionId());

    // Debugging output for EncryptionID
    std::cout << "EncryptFileWrapper - EncryptionID: "
              << globalDefinitions::toHexString(fileData[i].getEncryptionId(), fileData[i].getEncryptionIdLength())
              << std::endl;

    if (fileData[i].getKeyLength() == 16) { // AES-128
      futures.push_back(std::async(std::launch::async, AES128::encryptFile, &fileData[i]));
    } else if (fileData[i].getKeyLength() == 32) { // AES-256
      futures.push_back(std::async(std::launch::async, AES256::encryptFile, &fileData[i]));
    }
  }

  bool *resultArray = new bool[numFiles];
  for (int i = 0; i < numFiles; i++) {
    resultArray[i] = futures[i].get();
  }
  return resultArray;
}

[[maybe_unused]] CRYPTOLIB_API bool *DecryptFileWrapper(const FileData *fileData, const int numFiles) {
  std::vector<std::future<bool>> futures;
  for (int i = 0; i < numFiles; i++) {
    std::unique_ptr<FileData> file = std::make_unique<FileData>(fileData[i]);
    if (file->getKeyLength() == 16) { // AES-128
      futures.push_back(std::async(std::launch::async, AES128::decryptFile, file.get()));
    } else if (file->getKeyLength() == 32) { // AES-256
      futures.push_back(std::async(std::launch::async, AES256::decryptFile, file.get()));
    }
  }

  bool *resultArray = new bool[numFiles];
  for (int i = 0; i < numFiles; i++) {
    resultArray[i] = futures[i].get();
  }
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

[[maybe_unused]] CRYPTOLIB_API void getCurrentTimeHash(unsigned char *timeHash) {
  auto time = HelperUtils::getCurrentTime();
  auto hashedTime = SHA512::hashArray(time);

  std::copy(hashedTime.begin(), hashedTime.begin() + 64, timeHash);
}
}