//
// Created by prang on 23.11.2024.
//

#ifndef FILEVAULTROOT_KEYGEN_H
#define FILEVAULTROOT_KEYGEN_H

#include <vector>
#include <openssl/rand.h>
#include <stdexcept>
#include "../api/CryptoAPI.h"

/**
 * Key generation class for generating keys and initialization vectors (IV).
 */
class KeyGen {
public:
  /**
   * Generates a cryptographic key and initialization vector (IV).
   *
   * @param keySize The size of the key to generate.
   * @param ivSize The size of the IV to generate.
   * @param KEY A reference to a vector where the generated key will be stored.
   * @param IV A reference to a vector where the generated IV will be stored.
   * @throws std::runtime_error if key or IV generation fails.
   */
  void generateKeyIv(const size_t &keySize, const size_t &ivSize, std::vector<unsigned char> &KEY, std::vector<unsigned char> &IV);

  /**
   * Generates a specified number of random bytes.
   *
   * @param size The number of random bytes to generate.
   * @param randomBytes A reference to a vector where the generated random bytes will be stored.
   * @throws std::runtime_error if random byte generation fails.
   */
  void generateRandomBytes(const size_t &size, std::vector<unsigned char> &randomBytes);

  /**
   * Converts a vector of bytes to a hexadecimal string representation.
   *
   * @param data The vector of bytes to convert.
   * @return A string containing the hexadecimal representation of the input data.
   */
  static std::string toHexString(const std::vector<unsigned char> &data);

private:
  /// Flag to enable or disable debug printing.
  bool printDebug = true;
};

#endif //FILEVAULTROOT_KEYGEN_H