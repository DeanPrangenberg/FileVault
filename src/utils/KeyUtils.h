//
// Created by prang on 25.09.2024.
//

#ifndef RANSOMWARE_KEYUTILS_H
#define RANSOMWARE_KEYUTILS_H

#include "../../include/global.h"

class KeyUtils {
public:
  KeyIvAES generateKeyIv(int keySize);

  std::string bytesToInts(const std::vector<unsigned char>& bytes);

  std::vector<unsigned char> intsToBytes(const std::string& ints);
};


#endif //RANSOMWARE_KEYUTILS_H
