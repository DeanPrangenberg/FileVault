#ifndef GLOBALDEFINITIONS_H
#define GLOBALDEFINITIONS_H

#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <algorithm>


namespace globalDefinitions {
  // variables
  extern const std::wstring encFileSuffix;
  extern const std::string markIdentifier;

  // enums
  enum AlgorithmType {
    AES128,
    AES256
  };

  // functions
  inline std::string toHexString(const unsigned char *pUChar, size_t length) {
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
}
#endif // GLOBALDEFINITIONS_H