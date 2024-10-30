#ifndef ROOTPROJECT_SAVEFILEOPERATIONS_H
#define ROOTPROJECT_SAVEFILEOPERATIONS_H

#include <vector>
#include <string>
#include <Windows.h>
#include <optional>
#include <nlohmann/json.hpp>
#include "../../CppFileVaultMain/src/GlobalDefinitions.h"

#ifdef SAVEFILELIB_EXPORTS
#define SAVEFILELIB_API __declspec(dllexport)
#else
#define SAVEFILELIB_API __declspec(dllimport)
#endif

using globalDefinitions::FileData;
using json = nlohmann::json;

class SaveFileLib {
public:
  std::optional<FileData> findMatchingFileData(const FileData *incompleteData, const std::vector<FileData> &fileDataList);
  bool compareFileData(const FileData *a, const FileData &b);
  static void createFileIfNotExists(const wchar_t *filePath);
  std::vector<FileData> readFileDataFromJson(const wchar_t *filePath);
  std::string toBinaryString(const unsigned char *data, int length);
  std::string wstringToUtf8(const wchar_t *wstr);
  std::vector<unsigned char> fromBinaryString(const std::string &binaryString);
  std::wstring utf8ToWstring(const std::string &str);
  FileData deserializeJsonToFileData(const json &j);
  json serializeFileDataToJson(const FileData &fileData);
};

const bool printDebug = false;

extern "C" {
[[maybe_unused]] SAVEFILELIB_API void WriteFileDataToJson(const FileData *fileDataList,
                                                          size_t fileDataSize,
                                                          const wchar_t *saveFilePath);
[[maybe_unused]] SAVEFILELIB_API bool FindAndCompleteStruct(FileData *partialStruct, const wchar_t *saveFilePath);
[[maybe_unused]] SAVEFILELIB_API bool DeleteStructFromJson(const FileData *targetStruct, const wchar_t *saveFilePath);
}

#endif //ROOTPROJECT_SAVEFILEOPERATIONS_H