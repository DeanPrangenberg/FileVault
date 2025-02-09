//
// Created by prang on 20.11.2024.
//

#ifndef FILEVAULTROOT_MASTERDLLCLASS_H
#define FILEVAULTROOT_MASTERDLLCLASS_H

#include <windows.h>
#include <filesystem>
#include <iostream>
#include "../../../shared/src/GlobalDefinitions.h"
#include "../../../shared/src/settingsClasses/Logs.h"

namespace fs = std::filesystem;

/**
 * @class MasterDLLClass
 * @brief A base class for handling DLL operations.
 */
class MasterDLLClass {
protected:
  /**
   * @brief Loads a DLL with the given name.
   *
   * @param dllName The name of the DLL to load.
   * @return A handle to the loaded DLL, or nullptr if the DLL could not be loaded.
   */
  static HMODULE loadDll(const wchar_t *dllName) {
    HMODULE hDll = LoadLibraryW(dllName);
    if (!hDll) {
      std::wcerr << L"Failed to load DLL: " << dllName << std::endl;
    }
    return hDll;
  }

  /**
   * @brief Unloads the specified DLL.
   *
   * @param hDll A handle to the DLL to unload.
   */
  static void unloadDll(HMODULE hDll) {
    if (hDll) {
      FreeLibrary(hDll);
    }
  }
};

#endif //FILEVAULTROOT_MASTERDLLCLASS_H