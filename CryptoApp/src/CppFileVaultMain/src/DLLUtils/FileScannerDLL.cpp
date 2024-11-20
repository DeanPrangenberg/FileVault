//
// Created by prang on 20.11.2024.
//

#include "FileScannerDLL.h"

std::vector<fs::path> FileScannerDLL::ScanDirectory(const fs::path& directory, const bool searchOnlyForDecryptedFiles) {
  HMODULE hDirectoryScannerDll = loadDll(L"CppDirectoryScannerLib.dll");
  if (!hDirectoryScannerDll) {
    return {};
  }

  auto scanDirectoryFunc = (ScanDirectoryFunc) GetProcAddress(hDirectoryScannerDll, "ScanForFilesInDirectory");
  if (!scanDirectoryFunc) {
    logError("Failed to get function address for GenerateFileID");
    unloadDll(hDirectoryScannerDll);
    return {};
  }

  const wchar_t **filePaths = scanDirectoryFunc(directory.wstring().c_str(), searchOnlyForDecryptedFiles);

  std::vector<fs::path> paths;
  for (size_t i = 0; filePaths[i] != nullptr; ++i) {
    paths.push_back(fs::path(filePaths[i]));
  }

  unloadDll(hDirectoryScannerDll);
  return paths;
}
