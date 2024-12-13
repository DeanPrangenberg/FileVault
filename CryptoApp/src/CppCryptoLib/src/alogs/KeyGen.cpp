#include "KeyGen.h"
#include <iomanip>
#include <sstream>

void KeyGen::generateKeyIv(const size_t &keySize, std::vector<unsigned char> &KEY, std::vector<unsigned char> &IV) {
  std::vector<unsigned char> key(keySize);
  const int ivSize = 16; // 128-bit IV
  std::vector<unsigned char> iv(ivSize);

  if (!RAND_bytes(key.data(), static_cast<int>(key.size())) || !RAND_bytes(iv.data(), static_cast<int>(iv.size()))) {
    throw std::runtime_error("Failed to generate key or IV");
  }

  KEY = key;
  IV = iv;

  if (printDebug) {
    std::cout << "Key and IV generated successfully" << std::endl;
    std::cout << "Key: " << toHexString(KEY) << std::endl;
    std::cout << "IV: " << toHexString(IV) << std::endl;
  }
}

std::string KeyGen::toHexString(const std::vector<unsigned char> &data) {
  std::ostringstream oss;
  for (const auto &byte : data) {
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }
  return oss.str();
}
