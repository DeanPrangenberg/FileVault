#ifndef FILEDATA_H
#define FILEDATA_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

class FileData {
public:
  /**
   * @brief Constructs a new FileData object.
   */
  FileData();

  /**
   * @brief Prints the FileData object in a debug format.
   */
  void debugFileData();

  /**
   * @brief Equality operator for FileData.
   *
   * Compares two FileData objects for equality based on their FileID and EncryptionID.
   *
   * @param other The other FileData object to compare with.
   * @return True if the FileID and EncryptionID are equal, false otherwise.
   */
  bool operator==(const FileData& other) const;

  /**
   * @brief Flag to enable or disable debug printing.
   */
  bool printDebug = true;

  /**
   * @brief Shared pointer to the FileID vector.
   */
  std::shared_ptr<std::vector<unsigned char>> FileID;

  /**
   * @brief Shared pointer to the EncryptionID vector.
   */
  std::shared_ptr<std::vector<unsigned char>> EncryptionID;

  /**
   * @brief Shared pointer to the LastUpdateID vector.
   */
  std::shared_ptr<std::vector<unsigned char>> LastUpdateID;

  /**
   * @brief Shared pointer to the algorithm type string.
   */
  std::shared_ptr<std::string> AlgorithmenType;

  /**
   * @brief Shared pointer to the original file path.
   */
  std::shared_ptr<fs::path> OriginalFilePath;

  /**
   * @brief Shared pointer to the encrypted file path.
   */
  std::shared_ptr<fs::path> EncryptedFilePath;

  /**
   * @brief Shared pointer to the decrypted file path.
   */
  std::shared_ptr<fs::path> DecryptedFilePath;

  /**
   * @brief Shared pointer to the key vector.
   */
  std::shared_ptr<std::vector<unsigned char>> Key;

  /**
   * @brief Shared pointer to the IV vector.
   */
  std::shared_ptr<std::vector<unsigned char>> Iv;

  /**
   * @brief Converts a vector of unsigned char to a hexadecimal string.
   *
   * @param data A shared pointer to the vector of unsigned char.
   * @return The hexadecimal string representation of the data.
   * @throws std::invalid_argument if the data is null or empty.
   */
  static std::string toHexString(const std::shared_ptr<std::vector<unsigned char>>& data);
};

#endif // FILEDATA_H