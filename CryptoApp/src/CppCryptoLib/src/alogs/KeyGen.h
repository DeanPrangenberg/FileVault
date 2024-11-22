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
  static void generateKeyIv(const size_t &keySize, std::vector<unsigned char> &KEY, std::vector<unsigned char> &IV);
};

#endif //FILEVAULTROOT_KEYGEN_H
