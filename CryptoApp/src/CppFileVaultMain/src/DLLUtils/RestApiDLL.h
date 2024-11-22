#ifndef FILEVAULTROOT_RESTAPIDLL_H
#define FILEVAULTROOT_RESTAPIDLL_H

#include "MasterDLLClass.h"
#include <vector>

class RestApiDLL : MasterDLLClass {
public:
  bool InsertEntry(const FileData &data);
  bool DeleteEntry(const FileData &data);
  bool SearchEntry(FileData &data);
  bool ReplaceEntry(const FileData &data);
  std::vector<FileData> GetAllFileIDsAndEncryptedPaths();

private:
  struct FileDataDB {
    const wchar_t *FileID;
    const wchar_t *AlgorithmenType;
    const wchar_t *OriginalFilePath;
    const wchar_t *EncryptedFilePath;
    const wchar_t *DecryptedFilePath;
    const wchar_t *Key;
    const wchar_t *Iv;
  };

  FileDataDB convertFileDataToDBStruct(const FileData &data);
  FileData convertDBStructToFileData(const FileDataDB &data);

  wchar_t* convertToWChar(const unsigned char* input, size_t size);
  unsigned char* convertToUnsignedChar(const wchar_t* input, size_t size);

  typedef void (*InsertEntryFunc)(const FileData *, bool*);
  typedef void (*DeleteEntryFunc)(const FileData *, bool*);
  typedef void (*SearchEntryFunc)(const FileData *, bool*);
  typedef void (*ReplaceEntryFunc)(const FileData *, bool*);
  typedef void (*GetAllFileIDsAndEncryptedPathsFunc)(FileData **, bool*);

  wchar_t *binaryWCharToWChar(const wchar_t *input, size_t size);

  wchar_t *wcharToBinaryWChar(const wchar_t *input, size_t size);
};

#endif //FILEVAULTROOT_RESTAPIDLL_H