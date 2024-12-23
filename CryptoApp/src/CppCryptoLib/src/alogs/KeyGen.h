//
// Created by prang on 23.11.2024.
//

#ifndef FILEVAULTROOT_KEYGEN_H
#define FILEVAULTROOT_KEYGEN_H

#include <vector>
#include <openssl/rand.h>
#include <stdexcept>
#include "../api/CryptoAPI.h"

// Key generation class for generating keys and initialization vectors (IV)
class KeyGen {
public:
  void generateKeyIv(const size_t &keySize, const size_t &ivSize, std::vector<unsigned char> &KEY, std::vector<unsigned char> &IV);
  static std::string toHexString(const std::vector<unsigned char> &data);
private:
  bool printDebug = false;
};

#endif //FILEVAULTROOT_KEYGEN_H
