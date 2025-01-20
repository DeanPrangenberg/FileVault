// AES128.h
#ifndef FILEVAULTROOT_AES128_H
#define FILEVAULTROOT_AES128_H

#include <openssl/err.h>
#include <openssl/evp.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <future>
#include <mutex>
#include "../../../shared/src/GlobalDefinitions.h"
#include "../Helper/CryptoHelperUtils.h"
#undef min

namespace fs = std::filesystem;

/**
 * AES128 class provides static methods for encrypting and decrypting files using AES-128-CBC encryption.
 */
class AES128 {
public:
  /**
   * Encrypts a file using AES-128-CBC encryption.
   *
   * @param fileData Pointer to the FileData object containing file paths, key, and IV.
   * @return True if the encryption is successful, false otherwise.
   */
  static bool encryptFile(const FileData *fileData);

  /**
   * Decrypts a file using AES-128-CBC decryption.
   *
   * @param fileData Pointer to the FileData object containing file paths, key, and IV.
   * @return True if the decryption is successful, false otherwise.
   */
  static bool decryptFile(const FileData *fileData);

private:
  /// Mutex for synchronizing file operations.
  static std::mutex fileMutex;
};

#endif //FILEVAULTROOT_AES128_H