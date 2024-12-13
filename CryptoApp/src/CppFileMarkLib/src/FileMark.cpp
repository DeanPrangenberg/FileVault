#include "FileMark.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

extern "C" {
FILEMARKLIB_API bool extractIDsFromFile(const wchar_t *encryptedFilePath, unsigned char FileID[64], unsigned char EncryptionID[64]) {
  // Open the original file
  std::ifstream inputFile(encryptedFilePath, std::ios::binary);
  if (!inputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(encryptedFilePath);
    std::cout << "Failed to open original file for reading: " + filePathStr << std::endl;
    return false;
  }

  // Read the file contents
  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  // Find the positions of the markIdentifier
  std::string markIdentifier = globalDefinitions::markIdentifier;
  size_t firstStartPos = fileContents.find(markIdentifier);
  size_t secondStartPos = fileContents.find(markIdentifier, firstStartPos + markIdentifier.length());
  size_t thirdStartPos = fileContents.find(markIdentifier, secondStartPos + markIdentifier.length());

  size_t firstEndPos = fileContents.rfind(markIdentifier);
  size_t secondEndPos = fileContents.rfind(markIdentifier, firstEndPos - markIdentifier.length());
  size_t thirdEndPos = fileContents.rfind(markIdentifier, secondEndPos - markIdentifier.length());

  if (firstStartPos == std::string::npos || secondStartPos == std::string::npos || thirdStartPos == std::string::npos ||
      firstEndPos == std::string::npos || secondEndPos == std::string::npos || thirdEndPos == std::string::npos) {
    std::cout << "Required markIdentifier not found" << std::endl;
    return false;
  }

  // Extract IDs
  std::string fileIDStart = fileContents.substr(firstStartPos + markIdentifier.length(),
                                                secondStartPos - firstStartPos - markIdentifier.length());
  std::string encryptionIDStart = fileContents.substr(secondStartPos + markIdentifier.length(),
                                                      thirdStartPos - secondStartPos - markIdentifier.length());

  std::string encryptionIDEnd = fileContents.substr(secondEndPos + markIdentifier.length(),
                                              firstEndPos - secondEndPos - markIdentifier.length());
  std::string fileIDEnd = fileContents.substr(thirdEndPos + markIdentifier.length(),
                                                    secondEndPos - thirdEndPos - markIdentifier.length());

  // Convert IDs to hex format
  auto toHexString = [](const std::string &str) {
    std::ostringstream oss;
    oss << std::hex << std::setw(2) << std::setfill('0');
    for (unsigned char c : str) {
      oss << static_cast<int>(c);
    }
    return oss.str();
  };

  // Compare the IDs
  if (debugPrint) {
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
    std::cout << "& File ID Start: " << toHexString(fileIDStart) << std::endl;
    std::cout << "& File ID End: " << toHexString(fileIDEnd) << std::endl;
    std::cout << "& Encryption ID Start: " << toHexString(encryptionIDStart) << std::endl;
    std::cout << "& Encryption ID End: " << toHexString(encryptionIDEnd) << std::endl;
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
  }

  if (fileIDStart == fileIDEnd && encryptionIDStart == encryptionIDEnd) {
    if (fileIDStart.length() != 64 || encryptionIDStart.length() != 64) {
      std::cout << "File ID or Encryption ID has the wrong size" << std::endl;
      return false;
    }
    std::memcpy(FileID, fileIDStart.c_str(), fileIDStart.length());
    std::memset(FileID + fileIDStart.length(), 0, 64 - fileIDStart.length()); // Zero out the rest of the array
    std::memcpy(EncryptionID, encryptionIDStart.c_str(), encryptionIDStart.length());
    std::memset(EncryptionID + encryptionIDStart.length(), 0, 64 - encryptionIDStart.length()); // Zero out the rest of the array

    if (debugPrint) {
      std::cout << "Extracted File ID: " << toHexString(fileIDStart) << std::endl;
      std::cout << "Extracted Encryption ID: " << toHexString(encryptionIDStart) << std::endl;
    }
    return true;
  } else {
    std::cout << "File IDs or Encryption IDs do not match" << std::endl;
    return false;
  }
}

[[maybe_unused]] FILEMARKLIB_API void markFile(const FileData *fileData) {
  // Convert FileID and EncryptionID to string
  std::string fileIDStr(reinterpret_cast<const char *>(fileData->getFileId()), fileData->getFileIdLength());
  std::string encryptionIDStr(reinterpret_cast<const char *>(fileData->getEncryptionId()), fileData->getEncryptionIdLength());
  std::string fileMark = globalDefinitions::markIdentifier + fileIDStr + globalDefinitions::markIdentifier + encryptionIDStr + globalDefinitions::markIdentifier + "\n";

  // Open the original file
  std::ifstream inputFile(fileData->getEncryptedFilePath(), std::ios::binary);
  if (!inputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->getEncryptedFilePath());
    std::cout << "Failed to open original file for reading: " + filePathStr << std::endl;
    return;
  }

  // Read the file contents
  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  // Add the mark at the beginning and end
  std::string markedContents = fileMark + fileContents + fileMark;

  // Save the modified contents back to the file
  std::ofstream outputFile(fileData->getEncryptedFilePath(), std::ios::binary);
  if (!outputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->getEncryptedFilePath());
    std::cout << "Failed to open original file for writing: " + filePathStr << std::endl;
    return;
  }
  outputFile << markedContents;
  outputFile.close();
}

[[maybe_unused]] FILEMARKLIB_API bool unmarkFile(const FileData *fileData) {
  // Open the Encrypted file
  std::ifstream inputFile(fileData->getEncryptedFilePath(), std::ios::binary);
  if (!inputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->getEncryptedFilePath());
    std::cout << "Failed to open file for reading: " + filePathStr << std::endl;
    return false;
  }

  // Read the file contents
  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  // Extract the fileID and EncryptionID using extractIDsFromFile
  unsigned char extractedFileIDArr[64];
  unsigned char extractedEncryptionIDArr[64];
  if (!extractIDsFromFile(fileData->getEncryptedFilePath(), extractedFileIDArr, extractedEncryptionIDArr)) {
    std::cout << "Failed to extract file ID and Encryption ID" << std::endl;
    return false;
  }

  // Convert FileID and EncryptionID to string
  std::string fileIDStr(reinterpret_cast<const char *>(fileData->getFileId()), fileData->getFileIdLength());
  std::string encryptionIDStr(reinterpret_cast<const char *>(fileData->getEncryptionId()), fileData->getEncryptionIdLength());

  // Verify if the extracted fileID matches the fileID in the struct
  if (std::memcmp(fileData->getFileId(), extractedFileIDArr, fileData->getFileIdLength()) != 0) {
    std::cout << "File ID does not match" << std::endl;
    return false;
  }

  // Remove the marks from the file contents
  std::string markWithID = globalDefinitions::markIdentifier + fileIDStr + globalDefinitions::markIdentifier + encryptionIDStr + globalDefinitions::markIdentifier + "\n";

  // Remove the first occurrence
  size_t startPos = fileContents.find(markWithID);
  if (startPos != std::string::npos) {
    fileContents.erase(startPos, markWithID.length());
  } else {
    std::cout << "Failed to find the first mark identifier" << std::endl;
    return false;
  }

  // Remove the last occurrence
  size_t endPos = fileContents.rfind(markWithID);
  if (endPos != std::string::npos) {
    fileContents.erase(endPos, markWithID.length());
  } else {
    std::cout << "Failed to find the last mark identifier" << std::endl;
    return false;
  }

  // Save the modified contents back to the file
  std::ofstream outputFile(fileData->getEncryptedFilePath(), std::ios::binary);
  if (!outputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->getEncryptedFilePath());
    std::cout << "Failed to open original file for writing: " + filePathStr << std::endl;
    return false;
  }
  outputFile << fileContents;
  outputFile.close();

  return true;
}
}