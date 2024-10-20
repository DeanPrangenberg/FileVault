#ifndef DLLUTILS_H
#define DLLUTILS_H

#include <windows.h>
#include <vector>
#include <string>
#include "../GlobalDefinitions.h"

using globalDefinitions::FileData;

class DLLUtils {
public:
  typedef void (*GenerateKeyIvFunc)(size_t, unsigned char *, unsigned char *);
  typedef void (*EncryptFilesFunc)(struct FileData *, int);
  typedef void (*DecryptFilesFunc)(struct FileData *, int);
  typedef void (*WriteFileDataToJsonFunc)(const FileData *, size_t, const wchar_t *);
  typedef void (*GenerateFileIDFunc)(const wchar_t *, unsigned char **, int *);
  typedef const char* (*ScanDirectoryFunc)(const char *);
  typedef void (*FreeMemoryFunc)(void *);

  void GenerateKeyIv(size_t size, unsigned char *key, unsigned char *iv);
  void GenerateFileID(const wchar_t *filePath, unsigned char **fileID, int *fileIDLength);
  void EncryptFiles(struct FileData *fileData, int count);
  void DecryptFiles(struct FileData *fileData, int count);
  void WriteFileDataToJson(const std::vector<FileData> &fileDataList);
  std::vector<std::string> ProcessFileWithCsScanner(const char *filePath);

private:
  static HMODULE loadDll(const wchar_t *dllName);

  static void unloadDll(HMODULE hDll);

  static void logError(const std::string &message);

  static std::vector<std::string> SplitString(const std::string& str, char delimiter);
};

#endif // DLLUTILS_H