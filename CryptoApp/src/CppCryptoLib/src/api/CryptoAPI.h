#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <array>
#include <iostream>
#include <chrono>
#include <memory>
#include "../../../shared/FileData.h"
#include "../alogs/AES128.h"
#include "../alogs/AES256.h"
#include "../alogs/KeyGen.h"
#include "../alogs/SHA512.h"
#include "../../../shared/ThreadPool.h"

#ifdef CRYPTOLIBRARY_EXPORTS
#define CRYPTOLIB_API __declspec(dllexport)
#else
#define CRYPTOLIB_API __declspec(dllimport)
#endif

namespace fs = std::filesystem;

extern "C" {
[[maybe_unused]] CRYPTOLIB_API bool *EncryptFileWrapper(FileData *fileData, int numFiles);
[[maybe_unused]] CRYPTOLIB_API bool *DecryptFileWrapper(const FileData *fileData, int numFiles);
[[maybe_unused]] CRYPTOLIB_API void GenerateKeyIv(size_t keySize, size_t ivSize, unsigned char *key, unsigned char *iv);
[[maybe_unused]] CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char *fileID);
[[maybe_unused]] CRYPTOLIB_API void getCurrentTimeHash(unsigned char *timeHash);
}

#endif //CRYPTOLIB_H