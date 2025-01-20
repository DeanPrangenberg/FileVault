#include "CryptoAPI.h"

extern "C" {
  /**
   * Encrypts multiple files using AES-128 or AES-256 encryption based on the key length.
   *
   * @param fileData Pointer to an array of FileData objects containing file paths, keys, and IVs.
   * @param numFiles The number of files to encrypt.
   * @return A pointer to an array of booleans indicating the success of each file encryption.
   */
  [[maybe_unused]] CRYPTOLIB_API bool *EncryptFileWrapper(FileData *fileData, const int numFiles) {
    std::vector<std::future<bool>> futures;
    for (int i = 0; i < numFiles; i++) {
      getCurrentTimeHash(fileData[i].EncryptionID->data());

      if (std::string(fileData->AlgorithmenType->begin(), fileData->AlgorithmenType->end()) == "AES128") {
        futures.push_back(std::async(std::launch::async, AES128::encryptFile, &fileData[i]));
      } else if (std::string(fileData->AlgorithmenType->begin(), fileData->AlgorithmenType->end()) == "AES256") {
        futures.push_back(std::async(std::launch::async, AES256::encryptFile, &fileData[i]));
      } else {
        std::cerr << "Error: Invalid algorithm type." << std::endl;
        return nullptr;
      }
    }

    bool *resultArray = new bool[numFiles];
    for (int i = 0; i < numFiles; i++) {
      resultArray[i] = futures[i].get();
    }
    return resultArray;
  }

  /**
   * Decrypts multiple files using AES-128 or AES-256 decryption based on the key length.
   *
   * @param fileData Pointer to an array of FileData objects containing file paths, keys, and IVs.
   * @param numFiles The number of files to decrypt.
   * @return A pointer to an array of booleans indicating the success of each file decryption.
   */
  [[maybe_unused]] CRYPTOLIB_API bool *DecryptFileWrapper(const FileData *fileData, const int numFiles) {
    std::vector<std::future<bool>> futures;

    for (int i = 0; i < numFiles; i++) {
      if (std::string(fileData->AlgorithmenType->begin(), fileData->AlgorithmenType->end()) == "AES128") {
        futures.push_back(std::async(std::launch::async, AES128::decryptFile, &fileData[i]));
      } else if (std::string(fileData->AlgorithmenType->begin(), fileData->AlgorithmenType->end()) == "AES256") {
        futures.push_back(std::async(std::launch::async, AES256::decryptFile, &fileData[i]));
      } else {
        std::cerr << "Error: Invalid algorithm type." << std::endl;
        return nullptr;
      }
    }

    bool *resultArray = new bool[numFiles];
    for (int i = 0; i < numFiles; i++) {
      resultArray[i] = futures[i].get();
    }
    return resultArray;
  }

  /**
   * Generates a cryptographic key and initialization vector (IV).
   *
   * @param keySize The size of the key to generate.
   * @param ivSize The size of the IV to generate.
   * @param key Pointer to a buffer where the generated key will be stored.
   * @param iv Pointer to a buffer where the generated IV will be stored.
   */
  [[maybe_unused]] CRYPTOLIB_API void GenerateKeyIv(size_t keySize, size_t ivSize, unsigned char *key, unsigned char *iv) {
    KeyGen keyGen;

    if (key == nullptr || iv == nullptr) {
      std::cerr << "Error: key or iv pointer is null." << std::endl;
      return;
    }

    std::vector<unsigned char> keyVec;
    std::vector<unsigned char> ivVec;

    keyGen.generateKeyIv(keySize, ivSize, keyVec, ivVec);

    std::copy(keyVec.begin(), keyVec.end(), key);
    std::copy(ivVec.begin(), ivVec.end(), iv);
  }

  /**
   * Generates a unique file ID by computing the BLAKE2-512 hash of the file path.
   *
   * @param filePath The path to the file.
   * @param fileID Pointer to a buffer where the generated file ID will be stored.
   */
  [[maybe_unused]] CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char *fileID) {
    auto hash = BLAKE2::hashFile(filePath);
    auto hashStr = BLAKE2::hashString(std::wstring(filePath));

    std::vector<unsigned char> combinedHash;
    combinedHash.reserve(hash.size() + hashStr.size());
    combinedHash.insert(combinedHash.end(), hash.begin(), hash.end());
    combinedHash.insert(combinedHash.end(), hashStr.begin(), hashStr.end());

    std::array<unsigned char, EVP_MAX_MD_SIZE> hashID = BLAKE2::hashArray(combinedHash);

    std::copy(hashID.begin(), hashID.begin() + 64, fileID);
  }

  /**
   * Generates a hash of the current time.
   *
   * @param timeHash Pointer to a buffer where the generated time hash will be stored.
   */
  [[maybe_unused]] CRYPTOLIB_API void getCurrentTimeHash(unsigned char *timeHash) {
    auto time = CryptoHelperUtils::getCurrentTime();
    auto hashedTime = BLAKE2::hashArray(time);

    std::copy(hashedTime.begin(), hashedTime.begin() + 64, timeHash);
  }
}