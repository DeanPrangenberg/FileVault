#ifndef FILEVAULTROOT_RESTAPIDLL_H
#define FILEVAULTROOT_RESTAPIDLL_H

#include "MasterDLLClass.h"
#include <vector>
#include <iostream>
#include <stdexcept>

class RestApiDLL : MasterDLLClass {
public:
  bool InsertEntry(const FileData &data);
  bool DeleteEntry(const FileData &data);
  bool SearchEntry(FileData &data);
  bool ReplaceEntry(const FileData &data);

  std::vector<FileData> GetAllFileIDsAndEncryptedPaths();

private:
  struct FileDataDB {
    wchar_t *FileID;
    wchar_t *AlgorithmenType;
    wchar_t *OriginalFilePath;
    wchar_t *EncryptedFilePath;
    wchar_t *DecryptedFilePath;
    wchar_t *Key;
    wchar_t *Iv;
  };

  typedef void (*InsertEntryFunc)(const FileDataDB *, bool *);
  typedef void (*DeleteEntryFunc)(const FileDataDB *, bool *);
  typedef void (*SearchEntryFunc)(FileDataDB *, bool *);
  typedef void (*ReplaceEntryFunc)(const FileDataDB *, bool *);
  typedef void (*GetAllFileIDsAndEncryptedPathsFunc)(FileDataDB **, bool *);

  FileDataDB convertFileDataToDBStruct(const FileData &data);
  FileData convertDBStructToFileData(const FileDataDB &data);
  FileDataDB convertFileDataForSearch(const FileData &data);

  wchar_t *convertToWChar(const unsigned char *input, size_t size);
  unsigned char *convertToUnsignedChar(const wchar_t *input, size_t size);

  void debugFileDataDB(const FileDataDB &data);
};

#endif //FILEVAULTROOT_RESTAPIDLL_H