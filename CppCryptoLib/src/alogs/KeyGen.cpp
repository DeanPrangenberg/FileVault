//
// Created by prang on 14.10.2024.
//

#include <vector>
#include <openssl/rand.h>
#include <stdexcept>
#include "../CryptoNamespace.h"

using easyOpenSSL::KeyGen;

void KeyGen::generateKeyIv(const size_t &keySize, std::vector<unsigned char> &KEY, std::vector<unsigned char> &IV) {
  // Create the key and IV with specified sizes
  std::vector<unsigned char> key(keySize);
  const int ivSize = 16; // 128-bit IV
  std::vector<unsigned char> iv(ivSize);

  // Generate the key and IV
  if (!RAND_bytes(key.data(), key.size()) || !RAND_bytes(iv.data(), iv.size())) {
    throw std::runtime_error("Failed to generate key or IV");
  }

  // Assign the generated key and IV to the passed references
  KEY = key;
  IV = iv;
}
