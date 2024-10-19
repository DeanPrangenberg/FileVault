#include "CryptoNamespace.h"
#include <openssl/aes.h>
#include <vector>

namespace easyOpenSSL {
  // Wrapper functions for C compatibility
  extern "C" {
  CRYPTOLIB_API bool EncryptFileAes128(const char *originalFilePath, const char *encryptedFilePath,
                                       unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + ivSize);
    return AES128::encryptFile(originalFilePath, encryptedFilePath, keyVec, ivVec);
  }

  CRYPTOLIB_API bool DecryptFileAes128(const char *encryptedFilePath, const char *decryptedFilePath,
                                       unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + ivSize);
    return AES128::decryptFile(encryptedFilePath, decryptedFilePath, keyVec, ivVec);
  }

  CRYPTOLIB_API bool EncryptFileAes256(const char *originalFilePath, const char *encryptedFilePath,
                                       unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + ivSize);
    return AES256::encryptFile(originalFilePath, encryptedFilePath, keyVec, ivVec);
  }

  CRYPTOLIB_API bool DecryptFileAes256(const char *encryptedFilePath, const char *decryptedFilePath,
                                       unsigned char *key, size_t keySize, unsigned char *iv, size_t ivSize) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + ivSize);
    return AES256::decryptFile(encryptedFilePath, decryptedFilePath, keyVec, ivVec);
  }

  CRYPTOLIB_API void GenerateKeyIv(size_t keySize, unsigned char *key, unsigned char *iv) {
    std::vector<unsigned char> keyVec(key, key + keySize);
    std::vector<unsigned char> ivVec(iv, iv + 16); // 128-bit
    KeyGen::generateKeyIv(keySize, keyVec, ivVec);
  }
  }
}