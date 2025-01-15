//
// Created by prang on 23.11.2024.
//

#ifndef FILEVAULTROOT_HELPERUTILS_H
#define FILEVAULTROOT_HELPERUTILS_H

#include <filesystem>
#include <string>
#include <windows.h>
#include "../../../shared/src/GlobalDefinitions.h"
#include <iostream>
#include <openssl/err.h>
#include "../api/CryptoAPI.h"

namespace fs = std::filesystem;

class CryptoHelperUtils {
public:
  static void printError(const std::wstring &msg);
  static void deleteFile(const fs::path &filePath);
  static void MarkFile(const class FileData *fileData);
  static bool UnmarkFile(const class FileData *fileData);
  static std::vector<unsigned char> getCurrentTime();
private:
  typedef void (*MarkFileFunc)(const class FileData *);
  typedef bool (*UnmarkFileFunc)(const class FileData *);
  static HMODULE loadDll(const wchar_t *dllName);
  static void unloadDll(HMODULE hDll);
  static void logError(const std::string &message);
};

#endif //FILEVAULTROOT_HELPERUTILS_H
