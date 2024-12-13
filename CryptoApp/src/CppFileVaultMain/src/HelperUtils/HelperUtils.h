#ifndef FILEVAULTROOT_HELPERUTILS_H
#define FILEVAULTROOT_HELPERUTILS_H

#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include "../GlobalDefinitions.h"
#include "../../../shared/FileData.h"

namespace fs = std::filesystem;

class HelperUtils {
public:
  void repairLostEncFileStructs(std::vector<fs::path> &directorys);
  void repairAllLostStruct();

  struct FileIDData {
    std::array<unsigned char, 64> FileID;
    std::array<unsigned char, 64> EncryptionID;
    fs::path newEncryptedFilePath;
  };
private:
  bool printDebug = true;

  std::vector<fs::path> scanForAllFiles(const std::vector<fs::path> &directorys);
  std::vector<FileIDData> checkFilesForFileID(const std::vector<fs::path> &totalFiles);
  std::vector<FileData> findFileDataStructsInDatabase(const std::vector<FileIDData> &lostFiles);
  void updateFileDataInDB(std::vector<FileData> &fileDataList, const std::vector<FileIDData> &lostFiles, unsigned char *updateId);
};

#endif //FILEVAULTROOT_HELPERUTILS_H