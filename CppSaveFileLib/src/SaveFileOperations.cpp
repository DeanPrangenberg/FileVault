#include "SaveFileOperations.h"
#include <optional>
#include <vector>
#include <cstring>

bool SaveFileLib::compareFileData(const FileData *a, const FileData &b) {
  if (a->FileID && b.FileID && memcmp(a->FileID, b.FileID, a->fileIDLength) == 0) {
    return true;
  }
  if (a->EncryptedFilePath && b.EncryptedFilePath && wcscmp(a->EncryptedFilePath, b.EncryptedFilePath) == 0) {
    return true;
  }
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
