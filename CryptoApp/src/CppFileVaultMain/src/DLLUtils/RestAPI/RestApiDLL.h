#ifndef FILEVAULTROOT_RESTAPIDLL_H
#define FILEVAULTROOT_RESTAPIDLL_H

#include "MasterDLLClass.h"
#include "../../../../shared/FileData.h"
#include <vector>
#include <iostream>
#include <stdexcept>

class RestApiDLL : MasterDLLClass {
public:
  bool InsertEntry(const FileData &data);
  bool DeleteEntry(const FileData &data);
  bool SearchEntry(FileData &data);
  bool ReplaceEntry(const FileData &data);
  int getDatabaseFileSize();

  std::vector<FileData> GetAllFileIDsAndEncryptedPaths();

private:
  bool printDebug = true;
  bool printConverterDebug = true;
  struct FileDataDB {
    wchar_t *FileID = nullptr;
    size_t FileIDLength = 0;
    wchar_t *EncryptedID = nullptr;
    size_t EncryptedIDLength = 0;
    wchar_t *LastUpdateID = nullptr;
    size_t LastUpdateIDLength = 0;
    wchar_t *AlgorithmenType = nullptr;
    wchar_t *OriginalFilePath = nullptr;
    wchar_t *EncryptedFilePath = nullptr;
    wchar_t *DecryptedFilePath = nullptr;
    wchar_t *Key = nullptr;
    size_t KeyLength = 0;
    wchar_t *Iv = nullptr;
    size_t IvLength = 0;
  };

  typedef void (*InsertEntryFunc)(const FileDataDB *, bool *);
  typedef void (*DeleteEntryFunc)(const FileDataDB *, bool *);
  typedef void (*SearchEntryFunc)(FileDataDB *, bool *);
  typedef void (*ReplaceEntryFunc)(const FileDataDB *, bool *);
  typedef void (*GetAllFileIDsAndEncryptedPathsFunc)(FileDataDB **, int *);
  typedef void (*GetDatabaseFileSizeFunc)(int *);

  FileDataDB convertFileDataToDBStruct(const FileData &data);
  FileData convertDBStructToFileData(const FileDataDB &data);
  FileDataDB convertFileDataForSearch(const FileData &data);

  wchar_t *convertToHexWChar(const unsigned char *input, size_t size);
  unsigned char *convertFromHexWChar(const wchar_t *input, size_t &size);

  void debugFileDataDB(const FileDataDB &data);
};

#endif //FILEVAULTROOT_RESTAPIDLL_H