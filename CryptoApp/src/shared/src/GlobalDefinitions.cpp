#include "GlobalDefinitions.h"

const std::wstring globalDefinitions::encFileSuffix = L".encFV";
const std::string globalDefinitions::markIdentifier = "<FileVault>";

bool globalDefinitions::deleteFileAfterEncryption = false;
bool globalDefinitions::deleteFileAfterDecryption = false;

std::string globalDefinitions::toHexString(const unsigned char *pUChar, size_t length) {
  if (pUChar == nullptr) {
    throw std::invalid_argument("Null pointer passed to toHexString");
  }

  // Optimierte String-Speicherreservierung
  std::ostringstream oss;
  oss << std::hex << std::setw(2) << std::setfill('0');

  for (size_t i = 0; i < length; ++i) {
    oss << static_cast<int>(pUChar[i]);
  }
  return oss.str();
}
