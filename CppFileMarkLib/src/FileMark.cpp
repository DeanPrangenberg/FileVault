//
// Created by prang on 23.10.2024.
//

#include "FileMark.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>

extern "C" {
FILEMARKLIB_API bool extractFileIDFromFile(const wchar_t *encryptedFilePath, unsigned char FileID[64]) {
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

  // Find the positions of the markIdentifier twice from the beginning
  std::string markIdentifier = globalDefinitions::markIdentifier;
  size_t firstStartPos = fileContents.find(markIdentifier);
  if (firstStartPos == std::string::npos) {
    std::cout << "First start markIdentifier not found" << std::endl;
    return false;
  }
  size_t secondStartPos = fileContents.find(markIdentifier, firstStartPos + markIdentifier.length());
  if (secondStartPos == std::string::npos) {
    std::cout << "Second start markIdentifier not found" << std::endl;
    return false;
  }
  std::string fileIDStart = fileContents.substr(firstStartPos + markIdentifier.length(),
                                                secondStartPos - firstStartPos - markIdentifier.length());

  // Find the positions of the markIdentifier twice from the end
  size_t firstEndPos = fileContents.rfind(markIdentifier);
  if (firstEndPos == std::string::npos) {
    std::cout << "First end markIdentifier not found" << std::endl;
    return false;
  }
  size_t secondEndPos = fileContents.rfind(markIdentifier, firstEndPos - markIdentifier.length());
  if (secondEndPos == std::string::npos) {
    std::cout << "Second end markIdentifier not found" << std::endl;
    return false;
  }
  std::string fileIDEnd = fileContents.substr(secondEndPos + markIdentifier.length(),
                                              firstEndPos - secondEndPos - markIdentifier.length());

  // Compare the fileIDs and return if they are equal
  if (fileIDStart == fileIDEnd) {
    std::memcpy(FileID, fileIDStart.c_str(), fileIDStart.length() + 1);
    return true;
  } else {
    std::cout << "File IDs do not match" << std::endl;
    return false;
  }
}

FILEMARKLIB_API void markFile(const struct FileData *fileData) {
  // Convert FileID to string
  std::string fileIDStr(reinterpret_cast<const char *>(fileData->FileID), fileData->fileIDLength);
  std::string fileMark = globalDefinitions::markIdentifier + fileIDStr + globalDefinitions::markIdentifier + "\n";

  // Open the original file
  std::ifstream inputFile(fileData->EncryptedFilePath, std::ios::binary);
  if (!inputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->EncryptedFilePath);
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
  std::ofstream outputFile(fileData->EncryptedFilePath, std::ios::binary);
  if (!outputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->EncryptedFilePath);
    std::cout << "Failed to open original file for writing: " + filePathStr << std::endl;
    return;
  }
  outputFile << markedContents;
  outputFile.close();
}

FILEMARKLIB_API bool unmarkFile(const struct FileData *fileData) {
  // Open the original file
  std::ifstream inputFile(fileData->EncryptedFilePath, std::ios::binary);
  if (!inputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->EncryptedFilePath);
    std::cout << "Failed to open file for reading: " + filePathStr << std::endl;
    return false;
  }

  // Read the file contents
  std::ostringstream buffer;
  buffer << inputFile.rdbuf();
  std::string fileContents = buffer.str();
  inputFile.close();

  // Extract the fileID using extractFileID
  unsigned char extractedFileIDArr[64];
  if (extractFileIDFromFile(fileData->EncryptedFilePath, extractedFileIDArr)) {
    std::cout << "Failed to extract file ID" << std::endl;
    return false;
  }

  // Turn fileID in to a pointer
  auto *extractedFileID = new unsigned char[fileData->fileIDLength];

  // Convert FileID to string
  std::string fileIDStr(reinterpret_cast<const char *>(fileData->FileID), fileData->fileIDLength);

  // Verify if the extracted fileID matches the fileID in the struct
  if (std::memcmp(fileData->FileID, extractedFileID, fileData->fileIDLength) != 0) {
    std::cout << "File ID does not match" << std::endl;
    delete[] extractedFileID; // Free the allocated memory
    return false;
  }

  // Remove the marks from the file contents
  std::string markWithID = globalDefinitions::markIdentifier + fileIDStr + globalDefinitions::markIdentifier + "\n";

  // Remove the first occurrence
  size_t startPos = fileContents.find(markWithID);
  if (startPos != std::string::npos) {
    fileContents.erase(startPos, markWithID.length());
  } else {
    std::cout << "Failed to find the first mark identifier" << std::endl;
    delete[] extractedFileID; // Free the allocated memory
    return false;
  }

  // Remove the last occurrence
  size_t endPos = fileContents.rfind(markWithID);
  if (endPos != std::string::npos) {
    fileContents.erase(endPos, markWithID.length());
  } else {
    std::cout << "Failed to find the last mark identifier" << std::endl;
    delete[] extractedFileID; // Free the allocated memory
    return false;
  }

  // Save the modified contents back to the file
  std::ofstream outputFile(fileData->EncryptedFilePath, std::ios::binary);
  if (!outputFile) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(fileData->EncryptedFilePath);
    std::cout << "Failed to open original file for writing: " + filePathStr << std::endl;
    delete[] extractedFileID; // Free the allocated memory
    return false;
  }
  outputFile << fileContents;
  outputFile.close();

  // Free the allocated memory
  delete[] extractedFileID;
  return true;
}
}