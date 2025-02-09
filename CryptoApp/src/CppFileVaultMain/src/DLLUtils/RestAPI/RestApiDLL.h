#ifndef FILEVAULTROOT_RESTAPIDLL_H
#define FILEVAULTROOT_RESTAPIDLL_H

#include "MasterDLLClass.h"
#include "../../../../shared/src/FileData.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

/**
 * @class RestApiDLL
 * @brief A class that provides an interface to interact with the database using the GoRestApiWrapperLib DLL.
 */
class RestApiDLL : MasterDLLClass {
public:
  /**
   * @brief Exports the database using the GoRestApiWrapperLib DLL.
   *
   * @return A map containing the exported database data and key.
   */
  std::unordered_map<std::string, std::string> ExportDatabase();

  /**
   * @brief Inserts data into the database using the GoRestApiWrapperLib DLL.
   *
   * @param key The key for the data.
   * @param data The data to insert.
   * @return True if the insertion was successful, false otherwise.
   */
  bool InsertDatabase(const std::string &key, const std::string &data);

  /**
   * @brief Replaces data in the database using the GoRestApiWrapperLib DLL.
   *
   * @param key The key for the data.
   * @param data The data to replace.
   * @return True if the replacement was successful, false otherwise.
   */
  bool ReplaceDatabase(const std::string &key, const std::string &data);

  /**
   * @brief Resets the database using the GoRestApiWrapperLib DLL.
   *
   * @return True if the reset was successful, false otherwise.
   */
  bool ResetDatabase();

  /**
   * @brief Inserts an entry into the database using the GoRestApiWrapperLib DLL.
   *
   * @param data The FileData object to insert.
   * @return True if the insertion was successful, false otherwise.
   */
  bool InsertEntry(const FileData &data);

  /**
   * @brief Deletes an entry from the database using the GoRestApiWrapperLib DLL.
   *
   * @param data The FileData object to delete.
   * @return True if the deletion was successful, false otherwise.
   */
  bool DeleteEntry(const FileData &data);

  /**
   * @brief Searches for an entry in the database using the GoRestApiWrapperLib DLL.
   *
   * @param data The FileData object to search for.
   * @return True if the search was successful, false otherwise.
   */
  bool SearchEntry(FileData &data);

  /**
   * @brief Replaces an entry in the database using the GoRestApiWrapperLib DLL.
   *
   * @param data The FileData object to replace.
   * @return True if the replacement was successful, false otherwise.
   */
  bool ReplaceEntry(const FileData &data);

  /**
   * @brief Gets the size of the database file using the GoRestApiWrapperLib DLL.
   *
   * @return The size of the database file, or -1 if an error occurred.
   */
  int getDatabaseFileSize();

  /**
   * @brief Retrieves all FileData objects from the database using the GoRestApiWrapperLib DLL.
   *
   * @return A vector containing all FileData objects.
   */
  std::vector<FileData> GetAllFileIDsAndEncryptedPaths();

private:
  /**
   * @brief Flag to enable or disable debug printing.
   */
  bool printDebug = true;

  /**
   * @brief Flag to enable or disable converter debug printing.
   */
  bool printConverterDebug = true;

  /**
   * @brief Structure to hold file data for database operations.
   */
  struct FileDataDB {
    const wchar_t *FileID;
    size_t FileIDLength;
    const wchar_t *EncryptedID;
    size_t EncryptedIDLength;
    const wchar_t *LastUpdateID;
    size_t LastUpdateIDLength;
    const wchar_t *AlgorithmenType;
    const wchar_t *OriginalFilePath;
    const wchar_t *EncryptedFilePath;
    const wchar_t *DecryptedFilePath;
    const wchar_t *Key;
    size_t KeyLength;
    const wchar_t *Iv;
    size_t IvLength;
  };

  /**
   * @brief Function pointer type for inserting an entry into the database.
   */
  typedef void (*InsertEntryFunc)(const FileDataDB *, bool *);

  /**
   * @brief Function pointer type for deleting an entry from the database.
   */
  typedef void (*DeleteEntryFunc)(const FileDataDB *, bool *);

  /**
   * @brief Function pointer type for searching an entry in the database.
   */
  typedef void (*SearchEntryFunc)(FileDataDB *, bool *);

  /**
   * @brief Function pointer type for replacing an entry in the database.
   */
  typedef void (*ReplaceEntryFunc)(const FileDataDB *, bool *);

  /**
   * @brief Function pointer type for getting all file IDs and encrypted paths from the database.
   */
  typedef void (*GetAllFileIDsAndEncryptedPathsFunc)(FileDataDB **, int *);

  /**
   * @brief Function pointer type for getting the size of the database file.
   */
  typedef void (*GetDatabaseFileSizeFunc)(int *);

  /**
   * @brief Function pointer type for exporting the database.
   */
  typedef void (*ExportDatabaseFunc)(char **, int *, char **, int *, int *);

  /**
   * @brief Function pointer type for inserting data into the database.
   */
  typedef void (*InsertDatabaseFunc)(const char *, const char *, bool *);

  /**
   * @brief Function pointer type for replacing data in the database.
   */
  typedef void (*ReplaceDatabaseFunc)(const char *, const char *, bool *);

  /**
   * @brief Function pointer type for resetting the database.
   */
  typedef void (*ResetDatabaseFunc)(bool *);

  /**
   * @brief Converts a FileData object to a FileDataDB structure.
   *
   * @param data The FileData object to convert.
   * @return The resulting FileDataDB structure.
   */
  FileDataDB convertFileDataToDBStruct(const FileData &data);

  /**
   * @brief Converts a FileDataDB structure to a FileData object.
   *
   * @param data The FileDataDB structure to convert.
   * @return The resulting FileData object.
   */
  FileData convertDBStructToFileData(FileDataDB &data);

  /**
   * @brief Converts a FileData object to a FileDataDB structure for search purposes.
   *
   * @param data The FileData object to convert.
   * @return The resulting FileDataDB structure.
   */
  FileDataDB convertFileDataForSearch(const FileData &data);

  /**
   * @brief Converts an unsigned char array to a hex wchar_t string.
   *
   * @param input Pointer to the unsigned char array.
   * @param size Size of the array.
   * @return Pointer to the resulting hex wchar_t string.
   */
  wchar_t *convertToHexWChar(const unsigned char *input, size_t size);

  /**
   * @brief Converts a hex wchar_t string to an unsigned char array.
   *
   * @param input Pointer to the hex wchar_t string.
   * @param size Reference to the size of the resulting array.
   * @return Pointer to the resulting unsigned char array.
   */
  std::shared_ptr<std::vector<unsigned char>> convertFromHexWChar(const wchar_t *input);

  /**
   * @brief Prints debug information for a FileDataDB structure.
   *
   * @param data The FileDataDB structure to debug.
   */
  void debugFileDataDB(const FileDataDB &data);

  /**
   * @brief Converts a std::string to a wchar_t string.
   *
   * @param str The std::string to convert.
   * @return Pointer to the resulting wchar_t string.
   */
  wchar_t *convertStringToWChar(const std::string &str);

  /**
   * @brief Converts a wchar_t string to a std::string.
   *
   * @param wstr The wchar_t string to convert.
   * @return The resulting std::string.
   */
  std::string wcharToString(const wchar_t *wstr);

};

#endif //FILEVAULTROOT_RESTAPIDLL_H