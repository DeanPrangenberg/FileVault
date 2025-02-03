//
// Created by prang on 23.10.2024.
//

#include "CryptoHelperUtils.h"

/**
 * Prints an error message to the standard error stream.
 *
 * @param msg The error message to print.
 */
void CryptoHelperUtils::printError(const std::wstring &msg) {
  std::wcerr << msg << L" Error: " << ERR_reason_error_string(ERR_get_error()) << std::endl;
}

/**
 * Deletes a file based on its extension and global settings.
 *
 * @param filePath The path to the file to be deleted.
 */
void CryptoHelperUtils::deleteFile(const fs::path &filePath) {
  GetSettingsData settingsData;
  auto settings = settingsData.ReadBoolsFromSettings();
  try {
    if (filePath.extension() == globalDefinitions::encFileSuffix && settings["DeleteAfterEncryption"]) {
      std::cout << "Deleting original file after encryption: " << filePath << std::endl;
      fs::remove(filePath);
    } else if (settings["DeleteAfterDecryption"]) {
      std::cout << "Deleting encrypted file after decryption: " << filePath << std::endl;
      fs::remove(filePath);
    } else {
      std::cout << "Skipped file deletion for: " << filePath << std::endl;
      std::cout << (filePath.extension() == globalDefinitions::encFileSuffix ? "Encrypted File" : "Decrypted File") << std::endl;
      std::cout << "Delete after decryption: " << settings["DeleteAfterDecryption"] << std::endl;
      std::cout << "Delete after encryption: " << settings["DeleteAfterEncryption"] << std::endl;
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

/**
 * Marks a file using an external DLL.
 *
 * @param fileData Pointer to the FileData object containing file information.
 */
bool CryptoHelperUtils::MarkFile(const class FileData *fileData) {
  HMODULE hFileMarkDll = loadDll(L"CppFileMarkLib.dll");
  if (!hFileMarkDll) {
    return false;
  }

  auto markFileFunc = (MarkFileFunc) GetProcAddress(hFileMarkDll, "markFile");
  if (!markFileFunc) {
    logError("Failed to get function address for markFile");
    unloadDll(hFileMarkDll);
    return false;
  }

  if (!markFileFunc(fileData)) {
    return false;
  }

  unloadDll(hFileMarkDll);
  return true;
}

/**
 * Unmarks a file using an external DLL.
 *
 * @param fileData Pointer to the FileData object containing file information.
 * @return True if the file was successfully unmarked, false otherwise.
 */
bool CryptoHelperUtils::UnmarkFile(const class FileData *fileData) {
  HMODULE hFileMarkDll = loadDll(L"CppFileMarkLib.dll");
  if (!hFileMarkDll) {
    return false;
  }

  auto unmarkFileFunc = (UnmarkFileFunc) GetProcAddress(hFileMarkDll, "unmarkFile");
  if (!unmarkFileFunc) {
    logError("Failed to get function address for unmarkFile");
    unloadDll(hFileMarkDll);
    return false;
  }

  bool result = unmarkFileFunc(fileData);

  unloadDll(hFileMarkDll);

  return result;
}

/**
 * Gets the current time in milliseconds since epoch.
 *
 * @return A vector of unsigned char representing the current time in milliseconds.
 */
std::vector<unsigned char> CryptoHelperUtils::getCurrentTime() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  std::string ms_string = std::to_string(milliseconds);

  return std::vector<unsigned char>(ms_string.begin(), ms_string.end());
}

/**
 * Logs an error message to the standard error stream.
 *
 * @param message The error message to log.
 */
void CryptoHelperUtils::logError(const std::string &message) {
  std::cerr << message << std::endl;
}

/**
 * Loads a DLL.
 *
 * @param dllName The name of the DLL to load.
 * @return A handle to the loaded DLL.
 */
HMODULE CryptoHelperUtils::loadDll(const wchar_t *dllName) {
  HMODULE hDll = LoadLibraryW(dllName);
  if (!hDll) {
    std::wcerr << L"Failed to load DLL: " << dllName << std::endl;
  }
  return hDll;
}

/**
 * Unloads a DLL.
 *
 * @param hDll The handle to the DLL to unload.
 */
void CryptoHelperUtils::unloadDll(HMODULE hDll) {
  if (hDll) {
    FreeLibrary(hDll);
  }
}