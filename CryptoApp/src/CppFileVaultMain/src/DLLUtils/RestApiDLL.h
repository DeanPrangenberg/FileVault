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
  typedef void (*InsertEntryFunc)(const FileData *, bool result);
  typedef void (*DeleteEntryFunc)(const FileData *, bool result);
  typedef FileData *(*SearchEntryFunc)(const FileData *);
  typedef void (*ReplaceEntryFunc)(const FileData *, bool result);
  typedef void (*GetAllFileIDsAndEncryptedPathsFunc)(FileData **, bool result);
};

#endif //FILEVAULTROOT_RESTAPIDLL_H