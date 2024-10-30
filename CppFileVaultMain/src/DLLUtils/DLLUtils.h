#ifndef DLLUTILS_H
#define DLLUTILS_H

#include <windows.h>
#include <vector>
#include <string>
#include <filesystem>
#include "../GlobalDefinitions.h"

using globalDefinitions::FileData;
namespace fs = std::filesystem;

class DLLUtils {
public:
  void GenerateKeyIv(size_t size, unsigned char *key, unsigned char *iv);
  void GenerateFileID(const wchar_t *filePath, unsigned char *fileID);
  void EncryptFiles(struct FileData *fileData, int count, std::vector<bool> &results);
  void DecryptFiles(struct FileData *fileData, int count, std::vector<bool> &results);
  void WriteFileDataToJson(const std::vector<FileData> &fileDataList);
  bool FindAndCompleteStruct(FileData *partialStruct, const wchar_t *saveFilePath);
  bool DeleteStructFromJson(const FileData *targetStruct, const wchar_t *saveFilePath);
  std::vector<fs::path> ScanDirectory(const fs::path& directory, const bool searchOnlyForDecryptedFiles);
  bool ExtractFileIDFromFile(const wchar_t * filePath, unsigned char FileID[64]);

private:
  typedef void (*GenerateKeyIvFunc)(size_t, unsigned char *, unsigned char *);
  typedef bool* (*EncryptFilesFunc)(struct FileData *, int);
  typedef bool* (*DecryptFilesFunc)(struct FileData *, int);
  typedef void (*WriteFileDataToJsonFunc)(const FileData *, size_t, const wchar_t *);
  typedef void (*GenerateFileIDFunc)(const wchar_t *, unsigned char *);
  typedef const wchar_t ** (*ScanDirectoryFunc)(const wchar_t *, const bool);
  typedef bool (*DeleteStructFromJsonFunc)(const FileData *, const wchar_t *);
  typedef bool (*FindAndCompleteStructFunc)(FileData *, const wchar_t *);
  typedef bool (*ExtractFileIDFromFileFunc)(const wchar_t *, unsigned char[64]);

  static HMODULE loadDll(const wchar_t *dllName);
  static void unloadDll(HMODULE hDll);
  static void logError(const std::string &message);
};

#endif // DLLUTILS_H