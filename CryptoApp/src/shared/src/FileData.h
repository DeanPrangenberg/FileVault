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
  FileData();

  void debugFileData();
  bool operator==(const FileData& other) const;

  bool printDebug = false;

  std::shared_ptr<std::vector<unsigned char>> FileID;
  std::shared_ptr<std::vector<unsigned char>> EncryptionID;
  std::shared_ptr<std::vector<unsigned char>> LastUpdateID;
  std::shared_ptr<std::string> AlgorithmenType;
  std::shared_ptr<fs::path> OriginalFilePath;
  std::shared_ptr<fs::path> EncryptedFilePath;
  std::shared_ptr<fs::path> DecryptedFilePath;
  std::shared_ptr<std::vector<unsigned char>> Key;
  std::shared_ptr<std::vector<unsigned char>> Iv;
  static std::string toHexString(const std::shared_ptr<std::vector<unsigned char>>& data);
};

#endif // FILEDATA_H