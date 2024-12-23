#include "KeyGen.h"
#include <iomanip>
#include <sstream>

void KeyGen::generateKeyIv(const size_t &keySize, const size_t &ivSize, std::vector<unsigned char> &KEY, std::vector<unsigned char> &IV) {
  KEY.resize(keySize);
  IV.resize(ivSize);

  if (!RAND_bytes(KEY.data(), static_cast<int>(keySize))
  || !RAND_bytes(IV.data(), static_cast<int>(ivSize))) {
    throw std::runtime_error("Failed to generate key or IV");
  }

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
