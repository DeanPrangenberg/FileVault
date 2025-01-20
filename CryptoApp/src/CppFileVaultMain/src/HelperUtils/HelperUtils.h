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

/**
 * @class HelperUtils
 * @brief Utility class for file encryption, decryption, and database management.
 */
class HelperUtils {
public:
  /**
   * @brief Encrypts the given files using the specified algorithms.
   * @param filePaths Vector of file paths to be encrypted.
   * @param algorithm Vector of encryption algorithms to be used.
   * @return Vector of integers representing the status of each encryption operation.
   */
  std::vector<int> encryptFiles(const std::vector<fs::path> &filePaths, const std::vector<std::string> &algorithm);

  /**
   * @brief Decrypts the given files.
   * @param filePaths Vector of file paths to be decrypted.
   * @return Unordered map with file paths as keys and decryption status as values.
   */
  std::unordered_map<std::string, int> decryptFiles(const std::vector<fs::path> &filePaths);

  /**
   * @brief Repairs lost encrypted file structures in the specified directories.
   * @param directorys Vector of directories to be scanned for lost structures.
   */
  void repairLostEncFileStructs(std::vector<fs::path> &directorys);

  /**
   * @brief Repairs all lost encrypted file structures.
   */
  void repairAllLostStruct();

  /**
   * @brief Saves the database to a file.
   * @param targetPath Path to the target file where the database will be saved.
   * @param database Unordered map representing the database to be saved.
   */
  void saveDatabaseToFile(const fs::path &targetDir, const std::unordered_map<std::string, std::string> &database);

  /**
   * @brief Loads the database from a file.
   * @param targetPath Path to the file from which the database will be loaded.
   * @return Unordered map representing the loaded database.
   */
  std::unordered_map<std::string, std::string> loadDatabaseFromFile(const fs::path &targetPath);

  /**
   * @struct FileIDData
   * @brief Structure to hold file ID and encryption ID data.
   */
  struct FileIDData {
    std::shared_ptr<std::vector<unsigned char>> FileID; ///< File ID
    std::shared_ptr<std::vector<unsigned char>> EncryptionID; ///< Encryption ID
    fs::path newEncryptedFilePath; ///< New encrypted file path
  };

  /**
   * @brief Checks the state of the database files.
   * @return Vector of integers representing the state of each database file.
   */
  std::vector<int> checkDBFileState();

private:
  bool printDebug = false; ///< Flag to enable or disable debug printing

  /**
   * @brief Converts a file path to a wide character string.
   * @param filePath File path to be converted.
   * @return Wide character string representing the file path.
   */
  wchar_t* convertPathToWchar(const fs::path &filePath);

  /**
   * @brief Scans the specified directories for all files.
   * @param directorys Vector of directories to be scanned.
   * @return Vector of file paths found in the directories.
   */
  std::vector<fs::path> scanForAllFiles(const std::vector<fs::path> &directorys);

  /**
   * @brief Checks the files for their file IDs.
   * @param totalFiles Vector of file paths to be checked.
   * @return Vector of FileIDData structures representing the file IDs.
   */
  std::vector<FileIDData> checkFilesForFileID(const std::vector<fs::path> &totalFiles);

  /**
   * @brief Finds file data structures in the database for the given lost files.
   * @param lostFiles Vector of FileIDData structures representing the lost files.
   * @return Vector of FileData structures found in the database.
   */
  std::vector<FileData> findFileDataStructsInDatabase(const std::vector<FileIDData> &lostFiles);

  /**
   * @brief Updates the file data in the database.
   * @param fileDataList Vector of FileData structures to be updated.
   * @param lostFiles Vector of FileIDData structures representing the lost files.
   * @param updateId Pointer to the update ID.
   */
  void updateFileDataInDB(std::vector<FileData> &fileDataList, const std::vector<FileIDData> &lostFiles,
                          std::vector<unsigned char> &updateId);
};

#endif //FILEVAULTROOT_HELPERUTILS_H