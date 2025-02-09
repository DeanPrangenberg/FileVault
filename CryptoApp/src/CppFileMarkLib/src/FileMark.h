#ifndef FILEVAULTROOT_FILEMARK_H
#define FILEVAULTROOT_FILEMARK_H

#include "../shared/src/GlobalDefinitions.h"
#include "../shared/src/FileData.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
#include <iomanip>

#ifdef FILEMARKLIB_EXPORTS
#define FILEMARKLIB_API __declspec(dllexport)
#else
#define FILEMARKLIB_API __declspec(dllimport)
#endif

extern "C" {
  /**
   * Extracts File ID and Encryption ID from an encrypted file.
   *
   * @param encryptedFilePath Pointer to the path of the encrypted file.
   * @param FileID Vector to store the extracted File ID.
   * @param EncryptionID Vector to store the extracted Encryption ID.
   * @return True if the IDs were successfully extracted, false otherwise.
   */
  FILEMARKLIB_API bool extractIDsFromFile(const fs::path *encryptedFilePath, std::vector<unsigned char> *FileID, std::vector<unsigned char> *EncryptionID);

  /**
   * Marks a file with File ID and Encryption ID.
   *
   * @param fileData Pointer to the FileData object containing file information.
   * @return True if the file was successfully marked, false otherwise.
   */
  FILEMARKLIB_API bool markFile(const FileData *fileData);

  /**
   * Unmarks a file by removing the File ID and Encryption ID.
   *
   * @param fileData Pointer to the FileData object containing file information.
   * @return True if the file was successfully unmarked, false otherwise.
   */
  FILEMARKLIB_API bool unmarkFile(const FileData *fileData);
}

/**
 * Class for marking and unmarking files with File ID and Encryption ID.
 */
class FileMarker {
public:
  /**
   * Extracts File ID and Encryption ID from an encrypted file.
   *
   * @param encryptedFilePath Pointer to the path of the encrypted file.
   * @param FileID Vector to store the extracted File ID.
   * @param EncryptionID Vector to store the extracted Encryption ID.
   * @return True if the IDs were successfully extracted, false otherwise.
   */
  static bool extractIDsFromFile(const fs::path *encryptedFilePath, std::vector<unsigned char> &FileID, std::vector<unsigned char> &EncryptionID);

  /**
   * Marks a file with File ID and Encryption ID.
   *
   * @param fileData Pointer to the FileData object containing file information.
   * @return True if the file was successfully marked, false otherwise.
   */
  static bool markFile(const FileData *fileData);

  /**
   * Unmarks a file by removing the File ID and Encryption ID.
   *
   * @param fileData Pointer to the FileData object containing file information.
   * @return True if the file was successfully unmarked, false otherwise.
   */
  static bool unmarkFile(const FileData *fileData);

private:
  /**
   * Flag to enable or disable debug printing.
   */
  static bool printDebug;

  /**
   * Converts a byte array to a hexadecimal string.
   *
   * @param pUChar Pointer to the byte array.
   * @param length Length of the byte array.
   * @return Hexadecimal string representation of the byte array.
   * @throws std::invalid_argument if the pointer is null.
   */
  static std::string toHexString(const unsigned char *pUChar, size_t length);

  /**
   * Converts a hexadecimal string to a byte array.
   *
   * @param hexStr Hexadecimal string.
   * @return Byte array representation of the hexadecimal string.
   * @throws std::invalid_argument if the hex string length is invalid or contains invalid characters.
   */
  static std::vector<unsigned char> fromHexString(const std::string &hexStr);
};

#endif //FILEVAULTROOT_FILEMARK_H