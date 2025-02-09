//
// Created by prang on 20.11.2024.
//

#ifndef FILEVAULTROOT_CRYPTODLL_H
#define FILEVAULTROOT_CRYPTODLL_H

#include "MasterDLLClass.h"
#include "../../../shared/src/FileData.h"
#include <thread>
#include <memory>

/**
 * @class CryptoDLL
 * @brief A class for cryptographic operations using the CppCryptoLib DLL.
 */
class CryptoDLL : MasterDLLClass {
public:
  /**
   * @brief Generates a key and initialization vector (IV) using the CppCryptoLib DLL.
   *
   * @param keySize The size of the key to generate.
   * @param ivSize The size of the IV to generate.
   * @param key Pointer to the buffer to store the generated key.
   * @param iv Pointer to the buffer to store the generated IV.
   */
  void GenerateKeyIv(size_t keySize, size_t ivSize, unsigned char *key, unsigned char *iv);

  /**
   * @brief Generates a file ID for a given file path using the CppCryptoLib DLL.
   *
   * @param filePath The path of the file to generate the ID for.
   * @param fileID Pointer to the buffer to store the generated file ID.
   */
  void GenerateFileID(const wchar_t *filePath, unsigned char *fileID);

  /**
   * @brief Encrypts multiple files using the CppCryptoLib DLL.
   *
   * @param fileData Pointer to an array of FileData objects representing the files to encrypt.
   * @param count The number of files to encrypt.
   * @param results A vector to store the results of the encryption for each file.
   */
  void EncryptFiles(class FileData *fileData, int count, std::vector<bool> &results);

  /**
   * @brief Decrypts multiple files using the CppCryptoLib DLL.
   *
   * @param fileData Pointer to an array of FileData objects representing the files to decrypt.
   * @param count The number of files to decrypt.
   * @param results A vector to store the results of the decryption for each file.
   */
  void DecryptFiles(class FileData *fileData, int count, std::vector<bool> &results);

  /**
   * @brief Generates a hash of the current time using the CppCryptoLib DLL.
   *
   * @param timeHash Pointer to the buffer to store the generated time hash.
   */
  void getCurrentTimeHash(unsigned char *timeHash);

  /**
   * @brief Generates a password hash using the CppCryptoLib DLL.
   *
   * @param password Pointer to the password buffer.
   * @param salt Pointer to the salt buffer.
   * @param hash Pointer to the buffer to store the generated hash.
   * @param passwordLength The length of the password.
   * @param saltLength The length of the salt.
   * @param hashSize The size of the hash to generate.
   */
  void GeneratePasswordHash(unsigned char *password, unsigned char *salt,
                            unsigned char *hash, size_t passwordLength, size_t saltLength, size_t hashSize);

  /**
   * @brief Generates random bytes using the CppCryptoLib DLL.
   *
   * @param randomBytesSize The size of the random bytes to generate.
   * @param randomBytes Pointer to the buffer to store the generated random bytes.
   */
  void generateRandomBytes(size_t randomBytesSize, unsigned char *randomBytes);

private:
  /**
   * @brief Function pointer type for generating a key and IV.
   */
  typedef void (*GenerateKeyIvFunc)(size_t, size_t, unsigned char *, unsigned char *);

  /**
   * @brief Function pointer type for generating a file ID.
   */
  typedef void (*GenerateFileIDFunc)(const wchar_t *, unsigned char *);

  /**
   * @brief Function pointer type for encrypting files.
   */
  typedef bool *(*EncryptFilesFunc)(class FileData *, int);

  /**
   * @brief Function pointer type for decrypting files.
   */
  typedef bool *(*DecryptFilesFunc)(class FileData *, int);

  /**
   * @brief Function pointer type for generating a password hash.
   */
  typedef void (*GeneratePasswordHashFunc)(unsigned char *, unsigned char *, unsigned char *, size_t, size_t, size_t);

  /**
   * @brief Function pointer type for generating a hash of the current time.
   */
  typedef void (*getCurrentTimeHashFunc)(unsigned char *);

  /**
   * @brief Function pointer type for generating random bytes.
   */
  typedef void (*GenerateRandomBytesFunc)(size_t, unsigned char *);
};

#endif //FILEVAULTROOT_CRYPTODLL_H