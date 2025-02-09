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
  /**
   * @brief Default constructor for globalDefinitions.
   */
  globalDefinitions() = default;

  // variables
  /**
   * @brief Suffix for encrypted files.
   */
  static const std::wstring encFileSuffix;

  /**
   * @brief Identifier mark for files.
   */
  static const std::string markIdentifier;

  /**
   * @brief Standard algorithm used for encryption.
   */
  static std::string stdAlgorithm;

  /**
   * @brief Flag to indicate whether to delete the file after encryption.
   */
  static bool deleteFileAfterEncryption;

  /**
   * @brief Flag to indicate whether to delete the file after decryption.
   */
  static bool deleteFileAfterDecryption;

  // enums
  /**
   * @brief Enumeration for algorithm types.
   */
  enum AlgorithmType {
    AES128, /**< AES-128 encryption algorithm */
    AES256  /**< AES-256 encryption algorithm */
  };

  // functions
  /**
   * @brief Converts an unsigned char array to a hex string.
   *
   * @param pUChar Pointer to the unsigned char array.
   * @param length Length of the array.
   * @return Hex string representation of the array.
   * @throws std::invalid_argument if pUChar is a null pointer.
   */
  static std::string toHexString(const unsigned char *pUChar, size_t length);

  /**
   * @brief Converts a hex string to a vector of unsigned char.
   *
   * @param hexStr The hex string to convert.
   * @return A vector of unsigned char representing the hex string.
   * @throws std::invalid_argument if the hex string length is invalid.
   */
  static std::vector<unsigned char> fromHexString(const std::string &hexStr);
};

#endif // GLOBALDEFINITIONS_H