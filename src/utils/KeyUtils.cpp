//
// Created by prang on 25.09.2024.
//

#include <openssl/rand.h>
#include "KeyUtils.h"

KeyIvAES KeyUtils::generateKeyIv(int keySize) {
  std::vector<unsigned char> key(keySize); // 128 bit key
  std::vector<unsigned char> iv(16); // 128 bit iv

  if (!RAND_bytes(key.data(), key.size()) || !RAND_bytes(iv.data(), iv.size())) {
    throw std::runtime_error("Failed to generate key or IV");
  }

  return KeyIvAES{key, iv};
}

std::string KeyUtils::bytesToInts(const std::vector<unsigned char>& bytes) {
  std::stringstream intStream;
  for (unsigned char byte : bytes) {
    intStream << static_cast<int>(byte) << ' '; // Konvertiere jedes Byte in int und füge es zum Stream hinzu
  }
  return intStream.str();
}


std::vector<unsigned char> KeyUtils::intsToBytes(const std::string& ints) {
  std::vector<unsigned char> bytes;
  std::stringstream intStream(ints);
  unsigned int byte;
  while (intStream >> byte) {
    bytes.push_back(static_cast<unsigned char>(byte)); // Konvertiere jede Ganzzahl zurück in ein Byte
  }
  return bytes;
}

