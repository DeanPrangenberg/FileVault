#include "FileMark.h"

bool FileMarker::printDebug = true;

extern "C" {
  /**
   * Extracts File ID and Encryption ID from an encrypted file.
   *
   * @param encryptedFilePath Pointer to the path of the encrypted file.
   * @param FileID Vector to store the extracted File ID.
   * @param EncryptionID Vector to store the extracted Encryption ID.
   * @return True if the IDs were successfully extracted, false otherwise.
   */
  FILEMARKLIB_API bool extractIDsFromFile(const fs::path* encryptedFilePath, std::vector<unsigned char> *FileID,
                                          std::vector<unsigned char> *EncryptionID) {
    return FileMarker::extractIDsFromFile(encryptedFilePath, *FileID, *EncryptionID);
  }

  /**
   * Marks a file with File ID and Encryption ID.
   *
   * @param fileData Pointer to the FileData object containing file information.
   * @return True if the file was successfully marked, false otherwise.
   */
  FILEMARKLIB_API bool markFile(const FileData *fileData) {
    return FileMarker::markFile(fileData);
  }

  /**
   * Unmarks a file by removing the File ID and Encryption ID.
   *
   * @param fileData Pointer to the FileData object containing file information.
   * @return True if the file was successfully unmarked, false otherwise.
   */
  FILEMARKLIB_API bool unmarkFile(const FileData *fileData) {
    return FileMarker::unmarkFile(fileData);
  }
}

/**
 * Extracts File ID and Encryption ID from an encrypted file.
 *
   * @param encryptedFilePath Pointer to the path of the encrypted file.
   * @param FileID Vector to store the extracted File ID.
   * @param EncryptionID Vector to store the extracted Encryption ID.
   * @return True if the IDs were successfully extracted, false otherwise.
 */
bool FileMarker::extractIDsFromFile(const fs::path *encryptedFilePath, std::vector<unsigned char> &FileID, std::vector<unsigned char> &EncryptionID) {
  std::ifstream inputFile(encryptedFilePath->wstring(), std::ios::binary);
  if (!inputFile) {
    std::cerr << "Failed to open original file for reading: " + encryptedFilePath->string() << std::endl;
    return false;
  }

  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  std::string markIdentifier = globalDefinitions::markIdentifier;
  size_t firstStartPos = fileContents.find(markIdentifier);
  size_t secondStartPos = fileContents.find(markIdentifier, firstStartPos + markIdentifier.length());
  size_t thirdStartPos = fileContents.find(markIdentifier, secondStartPos + markIdentifier.length());

  size_t firstEndPos = fileContents.rfind(markIdentifier);
  size_t secondEndPos = fileContents.rfind(markIdentifier, firstEndPos - markIdentifier.length());
  size_t thirdEndPos = fileContents.rfind(markIdentifier, secondEndPos - markIdentifier.length());

  if (firstStartPos == std::string::npos || secondStartPos == std::string::npos || thirdStartPos == std::string::npos ||
      firstEndPos == std::string::npos || secondEndPos == std::string::npos || thirdEndPos == std::string::npos) {
    std::cerr << "Required markIdentifier not found" << std::endl;
    return false;
  }

  std::string fileIDStartHex = fileContents.substr(firstStartPos + markIdentifier.length(),
                                                   secondStartPos - firstStartPos - markIdentifier.length());
  std::string encryptionIDStartHex = fileContents.substr(secondStartPos + markIdentifier.length(),
                                                         thirdStartPos - secondStartPos - markIdentifier.length());

  std::string encryptionIDEndHex = fileContents.substr(secondEndPos + markIdentifier.length(),
                                                       firstEndPos - secondEndPos - markIdentifier.length());
  std::string fileIDEndHex = fileContents.substr(thirdEndPos + markIdentifier.length(),
                                                 secondEndPos - thirdEndPos - markIdentifier.length());

  if (fileIDStartHex == fileIDEndHex && encryptionIDStartHex == encryptionIDEndHex) {
    std::vector<unsigned char> fileIDStart = fromHexString(fileIDStartHex);
    std::vector<unsigned char> encryptionIDStart = fromHexString(encryptionIDStartHex);

    if (fileIDStart.size() != 64 || encryptionIDStart.size() != 64) {
      std::cerr << "File ID or Encryption ID has the wrong size" << std::endl;
      std::cerr << "File ID size: " << fileIDStart.size() << std::endl;
      std::cerr << "Encryption ID size: " << encryptionIDStart.size() << std::endl;
      return false;
    }

    FileID.resize(fileIDStart.size());
    FileID.assign(fileIDStart.begin(), fileIDStart.end());

    EncryptionID.resize(encryptionIDStart.size());
    EncryptionID.assign(encryptionIDStart.begin(), encryptionIDStart.end());

    if (printDebug) {
      std::cout << "Extracted File ID: " << fileIDStartHex << std::endl;
      std::cout << "Extracted Encryption ID: " << encryptionIDStartHex << std::endl;
    }
    return true;
  } else {
    std::cerr << "File IDs or Encryption IDs do not match" << std::endl;
    return false;
  }
}

/**
 * Marks a file with File ID and Encryption ID.
 *
 * @param fileData Pointer to the FileData object containing file information.
 * @return True if the file was successfully marked, false otherwise.
 */
bool FileMarker::markFile(const FileData *fileData) {
  std::string fileMark = globalDefinitions::markIdentifier
      + toHexString(fileData->FileID->data(), fileData->FileID->size())
      + globalDefinitions::markIdentifier
      + toHexString(fileData->EncryptionID->data(), fileData->EncryptionID->size())
      + globalDefinitions::markIdentifier;

  std::ifstream inputFile(fileData->EncryptedFilePath->wstring(), std::ios::binary);
  if (!inputFile) {
    std::cerr << "Failed to open original file for reading: " + fileData->EncryptedFilePath->string() << std::endl;
    return false;
  }

  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  std::string markedContents = fileMark + fileContents + fileMark;

  std::ofstream outputFile(fileData->EncryptedFilePath->wstring(), std::ios::binary);
  if (!outputFile) {
    std::cerr << "Failed to open original file for writing: " + fileData->EncryptedFilePath->string() << std::endl;
    return false;
  }
  outputFile << markedContents;
  outputFile.close();
  return true;
}

/**
 * Unmarks a file by removing the File ID and Encryption ID.
 *
 * @param fileData Pointer to the FileData object containing file information.
 * @return True if the file was successfully unmarked, false otherwise.
 */
bool FileMarker::unmarkFile(const FileData *fileData) {

  std::string encryptionIdunmark = toHexString(fileData->EncryptionID->data(), fileData->EncryptionID->size());
  std::ifstream inputFile(fileData->EncryptedFilePath->wstring(), std::ios::binary);
  if (!inputFile) {
    std::cerr << "Failed to open file for reading: " + fileData->EncryptedFilePath->string() << std::endl;
    return false;
  }

  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  std::vector<unsigned char> extractedFileIDArr(64);
  std::vector<unsigned char> extractedEncryptionIDArr(64);
  if (!extractIDsFromFile(fileData->EncryptedFilePath.get(), extractedFileIDArr, extractedEncryptionIDArr)) {
    std::cerr << "Failed to extract file ID and Encryption ID" << std::endl;
    return false;
  }

  if (std::memcmp(fileData->FileID->data(), extractedFileIDArr.data(), fileData->FileID->size()) != 0) {
    std::cerr << "File ID does not match" << std::endl;
    return false;
  }

  std::string markWithID = globalDefinitions::markIdentifier
      + toHexString(fileData->FileID->data(), fileData->FileID->size())
      + globalDefinitions::markIdentifier
      + toHexString(fileData->EncryptionID->data(), fileData->EncryptionID->size())
      + globalDefinitions::markIdentifier;

  size_t startPos = fileContents.find(markWithID);
  if (startPos != std::string::npos) {
    fileContents.erase(startPos, markWithID.length());
  } else {
    std::cerr << "Failed to find the first mark identifier" << std::endl;
    std::cerr << "Mark with ID: " << markWithID << std::endl;
    std::cerr << "Found at position: ";
    for (size_t i = 0; i < markWithID.length(); ++i) {
      std::cerr << fileContents[i];
    }
    std::cerr << std::endl;
    return false;
  }

  size_t endPos = fileContents.rfind(markWithID);
  if (endPos != std::string::npos) {
    fileContents.erase(endPos, markWithID.length());
  } else {
    std::cerr << "Failed to find the last mark identifier" << std::endl;
    std::cerr << "Mark with ID: " << markWithID << std::endl;
    std::cerr << "Found at position: ";
    for (size_t i = 0; i < markWithID.length(); ++i) {
      std::cerr << fileContents[fileContents.length() - markWithID.length() + i];
    }
    std::cerr << std::endl;
    return false;
  }

  std::ofstream outputFile(fileData->EncryptedFilePath->wstring(), std::ios::binary);
  if (!outputFile) {
    std::cerr << "Failed to open original file for writing: " + fileData->EncryptedFilePath->string() << std::endl;
    return false;
  }
  outputFile << fileContents;
  outputFile.close();

  return true;
}

/**
 * Converts a byte array to a hexadecimal string.
 *
 * @param pUChar Pointer to the byte array.
 * @param length Length of the byte array.
 * @return Hexadecimal string representation of the byte array.
 * @throws std::invalid_argument if the pointer is null.
 */
std::string FileMarker::toHexString(const unsigned char *pUChar, size_t length) {
  if (pUChar == nullptr) {
    throw std::invalid_argument("Null pointer passed to toHexString");
  }

  std::ostringstream oss;
  oss << std::hex << std::setfill('0');

  for (size_t i = 0; i < length; ++i) {
    oss << std::setw(2) << static_cast<int>(pUChar[i]);
  }
  return oss.str();
}

/**
 * Converts a hexadecimal string to a byte array.
 *
 * @param hexStr Hexadecimal string.
 * @return Byte array representation of the hexadecimal string.
 * @throws std::invalid_argument if the hex string length is invalid or contains invalid characters.
 */
std::vector<unsigned char> FileMarker::fromHexString(const std::string &hexStr) {
  if (hexStr.length() % 2 != 0) {
    throw std::invalid_argument("Invalid hex string length");
  }

  std::vector<unsigned char> result;
  result.reserve(hexStr.length() / 2);

  for (size_t i = 0; i < hexStr.length(); i += 2) {
    char *end;
    long byte = std::strtol(hexStr.substr(i, 2).c_str(), &end, 16);
    if (*end != '\0' || byte < 0 || byte > 255) {
      throw std::invalid_argument("Invalid hex string");
    }
    result.push_back(static_cast<unsigned char>(byte));
  }

  return result;
}