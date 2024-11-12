#include "SaveFileOperations.h"
#include <optional>
#include <vector>
#include <cstring>
#include <iostream>

bool SaveFileLib::compareFileData(const FileData *a, const FileData &b) {
  std::cout << "FileData to find id: " << "" << std::endl;
  if (a->FileID && b.FileID && memcmp(a->FileID, b.FileID, a->fileIDLength) == 0) {
    return true;
  }
  std::cout << "FileID comparison failed" << std::endl;
  return false;
}

std::optional<FileData> SaveFileLib::findMatchingFileData(const FileData *incompleteData, const std::vector<FileData> &fileDataList) {
  for (const auto &fileData : fileDataList) {
    if (compareFileData(incompleteData, fileData)) {
      return fileData;
    }
  }
  return std::nullopt;
}
