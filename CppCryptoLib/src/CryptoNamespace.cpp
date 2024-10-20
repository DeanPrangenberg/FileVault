#include "CryptoNamespace.h"

namespace easyOpenSSL {
  // Wrapper functions for C compatibility
  extern "C" {
  CRYPTOLIB_API bool EncryptFileAes128(const wchar_t *originalFilePath, const wchar_t *encryptedFilePath,
                                       unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + ivSize);

    std::wstring originalFileWStr(originalFilePath);
    std::wstring encryptedFileWStr(encryptedFilePath);

    fs::path originalFileFsPath(originalFileWStr);
    fs::path encryptedFileFsPath(encryptedFileWStr);

    return AES128::encryptFile(originalFileFsPath, encryptedFileFsPath, keyVec, ivVec);
  }

  CRYPTOLIB_API bool DecryptFileAes128(const wchar_t *encryptedFilePath, const wchar_t *decryptedFilePath,
                                       unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + ivSize);

    std::wstring encryptedFileWStr(encryptedFilePath);
    std::wstring decryptedFileWStr(decryptedFilePath);

    fs::path encryptedFileFsPath(encryptedFileWStr);
    fs::path decryptedFileFsPath(decryptedFileWStr);

    return AES128::decryptFile(encryptedFileFsPath, decryptedFileFsPath, keyVec, ivVec);
  }

  CRYPTOLIB_API bool EncryptFileAes256(const wchar_t *originalFilePath, const wchar_t *encryptedFilePath,
                                       unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + ivSize);

    std::wstring originalFileWStr(originalFilePath);
    std::wstring encryptedFileWStr(encryptedFilePath);

    fs::path originalFileFsPath(originalFileWStr);
    fs::path encryptedFileFsPath(encryptedFileWStr);

    return AES256::encryptFile(originalFileFsPath, encryptedFileFsPath, keyVec, ivVec);
  }

  CRYPTOLIB_API bool DecryptFileAes256(const wchar_t *encryptedFilePath, const wchar_t *decryptedFilePath,
                                       unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + ivSize);

    std::wstring encryptedFileWStr(encryptedFilePath);
    std::wstring decryptedFileWStr(decryptedFilePath);

    fs::path encryptedFileFsPath(encryptedFileWStr);
    fs::path decryptedFileFsPath(decryptedFileWStr);

    return AES256::decryptFile(encryptedFileFsPath, decryptedFileFsPath, keyVec, ivVec);
  }

  CRYPTOLIB_API void GenerateKeyIv(size_t keySize, unsigned char *key, unsigned char *iv) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + 16); // 128-bit
    KeyGen::generateKeyIv(keySize, keyVec, ivVec);
  }

  CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char **fileID, int *fileIDLength) {
    SHA256::hashFile(filePath, fileID, fileIDLength);
  }

  }
}