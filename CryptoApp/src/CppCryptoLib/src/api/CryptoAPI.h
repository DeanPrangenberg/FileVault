#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <array>
#include <iostream>
#include <chrono>
#include <future>
#include <vector>
#include <memory>
#include "../../../shared/src/FileData.h"
#include "../../../shared/src/settingsClasses/Logs.h"
#include "../alogs/AES128.h"
#include "../alogs/AES256.h"
#include "../alogs/KeyGen.h"
#include "../alogs/SHA512.h"
#include "../alogs/BLAKE2.h"

#ifdef CRYPTOLIBRARY_EXPORTS
#define CRYPTOLIB_API __declspec(dllexport)
#else
#define CRYPTOLIB_API __declspec(dllimport)
#endif

namespace fs = std::filesystem;

extern "C" {
  /**
   * Encrypts multiple files using AES-128 or AES-256 encryption based on the key length.
   *
   * @param fileData Pointer to an array of FileData objects containing file paths, keys, and IVs.
   * @param numFiles The number of files to encrypt.
   * @return A pointer to an array of booleans indicating the success of each file encryption.
   */
  [[maybe_unused]] CRYPTOLIB_API bool *EncryptFileWrapper(FileData *fileData, int numFiles);

  /**
   * Decrypts multiple files using AES-128 or AES-256 decryption based on the key length.
   *
   * @param fileData Pointer to an array of FileData objects containing file paths, keys, and IVs.
   * @param numFiles The number of files to decrypt.
   * @return A pointer to an array of booleans indicating the success of each file decryption.
   */
  [[maybe_unused]] CRYPTOLIB_API bool *DecryptFileWrapper(const FileData *fileData, int numFiles);

  /**
   * Generates a cryptographic key and initialization vector (IV).
   *
   * @param keySize The size of the key to generate.
   * @param ivSize The size of the IV to generate.
   * @param key Pointer to a buffer where the generated key will be stored.
   * @param iv Pointer to a buffer where the generated IV will be stored.
   */
  [[maybe_unused]] CRYPTOLIB_API void GenerateKeyIv(size_t keySize, size_t ivSize, unsigned char *key, unsigned char *iv);

  /**
   * Generates a unique file ID by computing the SHA-512 hash of the file path.
   *
   * @param filePath The path to the file.
   * @param fileID Pointer to a buffer where the generated file ID will be stored.
   */
  [[maybe_unused]] CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char *fileID);

  /**
   * Generates a hash of the current time.
   *
   * @param timeHash Pointer to a buffer where the generated time hash will be stored.
   */
  [[maybe_unused]] CRYPTOLIB_API void getCurrentTimeHash(unsigned char *timeHash);
}

#endif //CRYPTOLIB_H