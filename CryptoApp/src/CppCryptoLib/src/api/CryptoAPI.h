#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#include <string>
#include <vector>
#include <filesystem>

#include <windows.h>
#include "../../../CppFileVaultMain/src/GlobalDefinitions.h"

#ifdef CRYPTOLIBRARY_EXPORTS
#define CRYPTOLIB_API __declspec(dllexport)
#else
#define CRYPTOLIB_API __declspec(dllimport)
#endif

namespace fs = std::filesystem;
using globalDefinitions::FileData;



// Wrapper functions for C compatibility
extern "C" {
[[maybe_unused]] CRYPTOLIB_API bool EncryptFileWrapper(const struct FileData *fileData);

[[maybe_unused]] CRYPTOLIB_API bool DecryptFileWrapper(const struct FileData *fileData);

[[maybe_unused]] CRYPTOLIB_API void GenerateKeyIv(size_t keySize, unsigned char *key, unsigned char *iv);

[[maybe_unused]] CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char *fileID);

}

#endif //CRYPTOLIB_H