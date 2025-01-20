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

/**
 * Utility class for various cryptographic helper functions.
 */
class CryptoHelperUtils {
public:
  /**
   * Prints an error message to the standard error stream.
   *
   * @param msg The error message to print.
   */
  static void printError(const std::wstring &msg);

  /**
   * Deletes a file based on its extension and global settings.
   *
   * @param filePath The path to the file to be deleted.
   */
  static void deleteFile(const fs::path &filePath);

  /**
   * Marks a file using an external DLL.
   *
   * @param fileData Pointer to the FileData object containing file information.
   */
  static bool MarkFile(const class FileData *fileData);

  /**
   * Unmarks a file using an external DLL.
   *
   * @param fileData Pointer to the FileData object containing file information.
   * @return True if the file was successfully unmarked, false otherwise.
   */
  static bool UnmarkFile(const class FileData *fileData);

  /**
   * Gets the current time in milliseconds since epoch.
   *
   * @return A vector of unsigned char representing the current time in milliseconds.
   */
  static std::vector<unsigned char> getCurrentTime();

private:
  /**
   * Function pointer type for marking a file.
   */
  typedef bool (*MarkFileFunc)(const class FileData *);

  /**
   * Function pointer type for unmarking a file.
   */
  typedef bool (*UnmarkFileFunc)(const class FileData *);

  /**
   * Loads a DLL.
   *
   * @param dllName The name of the DLL to load.
   * @return A handle to the loaded DLL.
   */
  static HMODULE loadDll(const wchar_t *dllName);

  /**
   * Unloads a DLL.
   *
   * @param hDll The handle to the DLL to unload.
   */
  static void unloadDll(HMODULE hDll);

  /**
   * Logs an error message to the standard error stream.
   *
   * @param message The error message to log.
   */
  static void logError(const std::string &message);
};

#endif //FILEVAULTROOT_HELPERUTILS_H