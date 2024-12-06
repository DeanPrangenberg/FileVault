//
// Created by prang on 23.10.2024.
//

#ifndef FILEVAULTROOT_FILEMARKDLL_H
#define FILEVAULTROOT_FILEMARK_H

#include "../../CppFileVaultMain/src/GlobalDefinitions.h"
#include "../../shared/FileData.h"

#ifdef FILEMARKLIB_EXPORTS
#define FILEMARKLIB_API __declspec(dllexport)
#else
#define FILEMARKLIB_API __declspec(dllimport)
#endif

const bool debugPrint = false;

extern "C" {
[[maybe_unused]] FILEMARKLIB_API bool extractFileIDFromFile(const wchar_t * filePath, unsigned char FileID[64]);
[[maybe_unused]] FILEMARKLIB_API void markFile(const FileData *fileData);
[[maybe_unused]] FILEMARKLIB_API bool unmarkFile(const FileData *fileData);
}

#endif //FILEVAULTROOT_FILEMARKDLL_H
