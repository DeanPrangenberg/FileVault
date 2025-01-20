//
// Created by prang on 23.11.2024.
//

#ifndef FILEVAULTROOT_SHA512_H
#define FILEVAULTROOT_SHA512_H

#include <openssl/evp.h>
#include <array>
#include <fstream>
#include <stdexcept>
#include <windows.h>
#include <vector>

/**
 * SHA512 class provides static methods for computing SHA-512 hashes of files, strings, and byte arrays.
 */
class SHA512 {
public:
  /**
   * Computes the SHA-512 hash of a file.
   *
   * @param filePath The path to the file to be hashed.
   * @return A std::array containing the hash value.
   * @throws std::runtime_error if the file cannot be opened or if any OpenSSL function fails.
   */
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashFile(const wchar_t *filePath);

  /**
   * Computes the SHA-512 hash of a wide string.
   *
   * @param input The wide string to be hashed.
   * @return A std::array containing the hash value.
   * @throws std::runtime_error if the string cannot be converted to UTF-8 or if any OpenSSL function fails.
   */
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashString(const std::wstring &input);

  /**
   * Computes the SHA-512 hash of a byte array.
   *
   * @param input The byte array to be hashed.
   * @return A std::array containing the hash value.
   * @throws std::runtime_error if any OpenSSL function fails.
   */
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashArray(const std::vector<unsigned char> &input);

private:
  /// Flag to enable or disable debug printing.
  bool printDebug = false;
};

#endif //FILEVAULTROOT_SHA512_H