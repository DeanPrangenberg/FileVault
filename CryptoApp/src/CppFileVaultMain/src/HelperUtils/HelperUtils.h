#ifndef FILEVAULTROOT_HELPERUTILS_H
#define FILEVAULTROOT_HELPERUTILS_H

#include <vector>
#include <array>
#include <iostream>
#include <codecvt>
#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <codecvt>
#include "../../../shared/src/GlobalDefinitions.h"
#include "../../../shared/src/FileData.h"
#include "../System/SystemUtils.h"
#include "../DLLUtils/FileScannerDLL.h"
#include "../DLLUtils/RestAPI/RestApiDLL.h"
#include "../DLLUtils/FileMarkDLL.h"
#include "../DLLUtils/CryptoDLL.h"
#include "../StructUtils/StructUtils.h"

namespace fs = std::filesystem;

class HelperUtils {
public:
  std::vector<int> encryptFiles(const std::vector<fs::path> &filePaths, const std::vector<std::string> &algorithm);
  std::unordered_map<std::string, int> decryptFiles(const std::vector<fs::path> &filePaths);
  void repairLostEncFileStructs(std::vector<fs::path> &directorys);
  void repairAllLostStruct();

  void saveDatabaseToFile(const fs::path &targetPath, std::unordered_map<std::string, std::string>);
  std::unordered_map<std::string, std::string> loadDatabaseFromFile(const fs::path &targetPath);

  struct FileIDData {
    std::array<unsigned char, 64> FileID;
    std::array<unsigned char, 64> EncryptionID;
    fs::path newEncryptedFilePath;
  };

  std::vector<int> checkDBFileState();

private:
  bool printDebug = true;

  wchar_t* convertPathToWchar(const fs::path &filePath);
  std::vector<fs::path> scanForAllFiles(const std::vector<fs::path> &directorys);
  std::vector<FileIDData> checkFilesForFileID(const std::vector<fs::path> &totalFiles);
  std::vector<FileData> findFileDataStructsInDatabase(const std::vector<FileIDData> &lostFiles);
  void updateFileDataInDB(std::vector<FileData> &fileDataList, const std::vector<FileIDData> &lostFiles, unsigned char *updateId);
};

#endif //FILEVAULTROOT_HELPERUTILS_H