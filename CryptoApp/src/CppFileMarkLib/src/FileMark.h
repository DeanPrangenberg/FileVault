#ifndef FILEVAULTROOT_FILEMARK_H
#define FILEVAULTROOT_FILEMARK_H

#include "../shared/src/GlobalDefinitions.h"
#include "../shared/src/FileData.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

#ifdef FILEMARKLIB_EXPORTS
#define FILEMARKLIB_API __declspec(dllexport)
#else
#define FILEMARKLIB_API __declspec(dllimport)
#endif

const bool debugPrint = false;

extern "C" {
[[maybe_unused]] FILEMARKLIB_API bool extractIDsFromFile(const wchar_t * filePath, unsigned char FileID[64], unsigned char EncryptionID[64]);
[[maybe_unused]] FILEMARKLIB_API void markFile(const FileData *fileData);
[[maybe_unused]] FILEMARKLIB_API bool unmarkFile(const FileData *fileData);
}

#endif //FILEVAULTROOT_FILEMARK_H