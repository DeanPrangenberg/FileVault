#include "GlobalDefinitions.h"

const std::wstring globalDefinitions::encFileSuffix = L".encFV";
const std::string globalDefinitions::markIdentifier = "<FileVault>";

bool globalDefinitions::deleteFileAfterEncryption = true;
bool globalDefinitions::deleteFileAfterDecryption = true;

/**
 * @brief Converts an unsigned char array to a hex string.
 *
 * @param pUChar Pointer to the unsigned char array.
 * @param length Length of the array.
 * @return Hex string representation of the array.
 * @throws std::invalid_argument if pUChar is a null pointer.
 */
std::string globalDefinitions::toHexString(const unsigned char *pUChar, size_t length) {
  if (pUChar == nullptr) {
    throw std::invalid_argument("Null pointer passed to toHexString");
  }

  // Optimized string memory reservation
  std::ostringstream oss;
  oss << std::hex << std::setw(2) << std::setfill('0');

  for (size_t i = 0; i < length; ++i) {
    oss << static_cast<int>(pUChar[i]);
  }
  return oss.str();
}

std::vector<unsigned char> globalDefinitions::fromHexString(const std::string &hexStr) {
  if (hexStr.length() % 2 != 0) {
    throw std::invalid_argument("Invalid hex string length");
  }

  std::vector<unsigned char> result;
  result.reserve(hexStr.length() / 2);

  for (size_t i = 0; i < hexStr.length(); i += 2) {
    unsigned int byte;
    std::istringstream(hexStr.substr(i, 2)) >> std::hex >> byte;
    result.push_back(static_cast<unsigned char>(byte));
  }

  return result;
}