//
// Created by prang on 23.10.2024.
//

#include <iostream>
#include <openssl/err.h>
#include "CryptoNamespace.h"


void HelperUtils::printError(const std::wstring &msg) {
  std::wcerr << msg << L" Error: " << ERR_reason_error_string(ERR_get_error()) << std::endl;
}

void HelperUtils::deleteFile(const fs::path &filePath) {
  try {
    if (fs::remove(filePath)) {
      std::cout << "File deleted successfully." << std::endl;
    } else {
      std::cout << "File not found." << std::endl;
    }
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void HelperUtils::MarkFile(const struct FileData *fileData) {
  HMODULE hFileMarkDll = loadDll(L"CppFileMarkLib.dll");
  if (!hFileMarkDll) {
    return;
  }

  auto markFileFunc = (MarkFileFunc) GetProcAddress(hFileMarkDll, "markFile");
  if (!markFileFunc) {
    logError("Failed to get function address for markFile");
    unloadDll(hFileMarkDll);
    return;
  }

  markFileFunc(fileData);
  unloadDll(hFileMarkDll);
}

bool HelperUtils::UnmarkFile(const struct FileData *fileData) {
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

void HelperUtils::logError(const std::string &message) {
  std::cerr << message << std::endl;
}

HMODULE HelperUtils::loadDll(const wchar_t *dllName) {
  HMODULE hDll = LoadLibraryW(dllName);
  if (!hDll) {
    std::wcerr << L"Failed to load DLL: " << dllName << std::endl;
  }
  return hDll;
}

void HelperUtils::unloadDll(HMODULE hDll) {
  if (hDll) {
    FreeLibrary(hDll);
  }
}