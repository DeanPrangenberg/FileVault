#include "FileMark.h"

extern "C" {
FILEMARKLIB_API bool extractIDsFromFile(const wchar_t *encryptedFilePath, unsigned char FileID[64], unsigned char EncryptionID[64]) {
  std::ifstream inputFile(encryptedFilePath, std::ios::binary);
  if (!inputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(encryptedFilePath);
    std::cout << "Failed to open original file for reading: " + filePathStr << std::endl;
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
    std::cout << "Required markIdentifier not found" << std::endl;
    return false;
  }

  std::string fileIDStart = fileContents.substr(firstStartPos + markIdentifier.length(),
                                                secondStartPos - firstStartPos - markIdentifier.length());
  std::string encryptionIDStart = fileContents.substr(secondStartPos + markIdentifier.length(),
                                                      thirdStartPos - secondStartPos - markIdentifier.length());

  std::string encryptionIDEnd = fileContents.substr(secondEndPos + markIdentifier.length(),
                                              firstEndPos - secondEndPos - markIdentifier.length());
  std::string fileIDEnd = fileContents.substr(thirdEndPos + markIdentifier.length(),
                                                    secondEndPos - thirdEndPos - markIdentifier.length());

  auto toHexString = [](const std::string &str) {
    std::ostringstream oss;
    oss << std::hex << std::setw(2) << std::setfill('0');
    for (unsigned char c : str) {
      oss << static_cast<int>(c);
    }
    return oss.str();
  };

  if (fileIDStart == fileIDEnd && encryptionIDStart == encryptionIDEnd) {
    if (fileIDStart.length() != 64 || encryptionIDStart.length() != 64) {
      std::cout << "File ID or Encryption ID has the wrong size" << std::endl;
      return false;
    }
    std::memcpy(FileID, fileIDStart.c_str(), fileIDStart.length());
    std::memset(FileID + fileIDStart.length(), 0, 64 - fileIDStart.length());
    std::memcpy(EncryptionID, encryptionIDStart.c_str(), encryptionIDStart.length());
    std::memset(EncryptionID + encryptionIDStart.length(), 0, 64 - encryptionIDStart.length());

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
  std::string fileIDStr(reinterpret_cast<const char *>(fileData->getFileId()), fileData->getFileIdLength());
  std::string encryptionIDStr(reinterpret_cast<const char *>(fileData->getEncryptionId()), fileData->getEncryptionIdLength());
  std::string fileMark = globalDefinitions::markIdentifier + fileIDStr + globalDefinitions::markIdentifier + encryptionIDStr + globalDefinitions::markIdentifier + "\n";

  std::ifstream inputFile(fileData->getEncryptedFilePath(), std::ios::binary);
  if (!inputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->getEncryptedFilePath());
    std::cout << "Failed to open original file for reading: " + filePathStr << std::endl;
    return;
  }

  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  std::string markedContents = fileMark + fileContents + fileMark;

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
  std::ifstream inputFile(fileData->getEncryptedFilePath(), std::ios::binary);
  if (!inputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->getEncryptedFilePath());
    std::cout << "Failed to open file for reading: " + filePathStr << std::endl;
    return false;
  }

  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  unsigned char extractedFileIDArr[64];
  unsigned char extractedEncryptionIDArr[64];
  if (!extractIDsFromFile(fileData->getEncryptedFilePath(), extractedFileIDArr, extractedEncryptionIDArr)) {
    std::cout << "Failed to extract file ID and Encryption ID" << std::endl;
    return false;
  }

  std::string fileIDStr(reinterpret_cast<const char *>(fileData->getFileId()), fileData->getFileIdLength());
  std::string encryptionIDStr(reinterpret_cast<const char *>(fileData->getEncryptionId()), fileData->getEncryptionIdLength());

  if (std::memcmp(fileData->getFileId(), extractedFileIDArr, fileData->getFileIdLength()) != 0) {
    std::cout << "File ID does not match" << std::endl;
    return false;
  }

  std::string markWithID = globalDefinitions::markIdentifier + fileIDStr + globalDefinitions::markIdentifier + encryptionIDStr + globalDefinitions::markIdentifier + "\n";

  size_t startPos = fileContents.find(markWithID);
  if (startPos != std::string::npos) {
    fileContents.erase(startPos, markWithID.length());
  } else {
    std::cout << "Failed to find the first mark identifier" << std::endl;
    return false;
  }

  size_t endPos = fileContents.rfind(markWithID);
  if (endPos != std::string::npos) {
    fileContents.erase(endPos, markWithID.length());
  } else {
    std::cout << "Failed to find the last mark identifier" << std::endl;
    return false;
  }

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