#ifndef BLAKE2_H
#define BLAKE2_H

#include <openssl/evp.h>
#include <array>
#include <fstream>
#include <stdexcept>
#include <vector>

/**
 * BLAKE2 class provides static methods for computing BLAKE2 hashes of files, strings, and byte arrays.
 */
class BLAKE2 {
public:
  /**
   * Computes the BLAKE2 hash of a file.
   *
   * @param filePath The path to the file to be hashed.
   * @return A std::array containing the hash value.
   * @throws std::runtime_error if the file cannot be opened or if any OpenSSL function fails.
   */
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashFile(const wchar_t *filePath);

  /**
   * Computes the BLAKE2 hash of a wide string.
   *
   * @param input The wide string to be hashed.
   * @return A std::array containing the hash value.
   * @throws std::runtime_error if the string cannot be converted to UTF-8 or if any OpenSSL function fails.
   */
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashString(const std::wstring &input);

  /**
   * Computes the BLAKE2 hash of a byte array.
   *
   * @param input The byte array to be hashed.
   * @return A std::array containing the hash value.
   * @throws std::runtime_error if any OpenSSL function fails.
   */
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashArray(const std::vector<unsigned char> &input);

  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashPassword(const std::vector<unsigned char> &password,
                                                                 const std::vector<unsigned char> &salt);

private:
  /// Flag to enable or disable debug printing.
  bool printDebug = true;
};

#endif // BLAKE2_H