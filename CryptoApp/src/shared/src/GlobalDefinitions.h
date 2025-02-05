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

class globalDefinitions {
public:
  globalDefinitions() = default;
  // variables
  static const std::wstring encFileSuffix;
  static const std::string markIdentifier;
  static std::string stdAlgorithm;
  static bool deleteFileAfterEncryption;
  static bool deleteFileAfterDecryption;

  // enums
  enum AlgorithmType {
    AES128,
    AES256
  };

  // functions
  static std::string toHexString(const unsigned char *pUChar, size_t length);

  static std::vector<unsigned char> fromHexString(const std::string &hexStr);
};
#endif // GLOBALDEFINITIONS_H