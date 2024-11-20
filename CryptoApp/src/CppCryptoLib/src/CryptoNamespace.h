#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

#include <string>
#include <vector>
#include <filesystem>
#include <openssl/evp.h>
#include <windows.h>
#include "../../CppFileVaultMain/src/GlobalDefinitions.h"

#ifdef CRYPTOLIBRARY_EXPORTS
#define CRYPTOLIB_API __declspec(dllexport)
#else
#define CRYPTOLIB_API __declspec(dllimport)
#endif

namespace fs = std::filesystem;
using globalDefinitions::FileData;

// AES-128 encryption class
class CRYPTOLIB_API AES128 {
public:
  static bool encryptFile(const FileData *fileData);
  static bool decryptFile(const FileData *fileData);
};

class CRYPTOLIB_API AES256 {
public:
  static bool encryptFile(const FileData *fileData);
  static bool decryptFile(const FileData *fileData);
};

class CRYPTOLIB_API SHA512 {
public:
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashFile(const wchar_t *filePath);
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hashString(const std::wstring &input);
};

// Key generation class for generating keys and initialization vectors (IV)
class CRYPTOLIB_API KeyGen {
public:
  static void generateKeyIv(const size_t &keySize, std::vector<unsigned char> &KEY, std::vector<unsigned char> &IV);
};

class HelperUtils {
public:
  static void printError(const std::wstring &msg);
  static void deleteFile(const fs::path &filePath);
  static void MarkFile(const struct FileData *fileData);
  static bool UnmarkFile(const struct FileData *fileData);
private:
  typedef void (*MarkFileFunc)(const struct FileData *);
  typedef bool (*UnmarkFileFunc)(const struct FileData *);
  static HMODULE loadDll(const wchar_t *dllName);
  static void unloadDll(HMODULE hDll);
  static void logError(const std::string &message);
};

// Wrapper functions for C compatibility
extern "C" {
[[maybe_unused]] CRYPTOLIB_API bool EncryptFileWrapper(const struct FileData *fileData);

[[maybe_unused]] CRYPTOLIB_API bool DecryptFileWrapper(const struct FileData *fileData);

[[maybe_unused]] CRYPTOLIB_API void GenerateKeyIv(size_t keySize, unsigned char *key, unsigned char *iv);

[[maybe_unused]] CRYPTOLIB_API void GenerateFileID(const wchar_t *filePath, unsigned char *fileID);

}

#endif //CRYPTOLIB_H