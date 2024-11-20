//
// Created by prang on 20.11.2024.
//

#ifndef FILEVAULTROOT_MASTERDLLCLASS_H
#define FILEVAULTROOT_MASTERDLLCLASS_H

#include <windows.h>
#include <filesystem>
#include <iostream>
#include "../GlobalDefinitions.h"

using globalDefinitions::FileData;
namespace fs = std::filesystem;

class MasterDLLClass {
protected:
  static HMODULE loadDll(const wchar_t *dllName) {
    HMODULE hDll = LoadLibraryW(dllName);
    if (!hDll) {
      std::wcerr << L"Failed to load DLL: " << dllName << std::endl;
    }
    return hDll;
  }
  static void unloadDll(HMODULE hDll) {
    if (hDll) {
      FreeLibrary(hDll);
    }
  }
  static void logError(const std::string &message) {
    std::cerr << message << std::endl;
  }
};

#endif //FILEVAULTROOT_MASTERDLLCLASS_H
