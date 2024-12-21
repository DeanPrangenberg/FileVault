#include "FileScannerDLL.h"

void FileScannerDLL::ScanDirectory(const std::filesystem::path& directory, const bool searchOnlyForEncFiles, std::vector<std::filesystem::path> &fileList, FileFoundCallback callback) {
  HMODULE hDirectoryScannerDll = loadDll(L"CppDirectoryScannerLib.dll");
  if (!hDirectoryScannerDll) {
    return;
  }

  auto scanDirectoryFunc = (ScanDirectoryFunc) GetProcAddress(hDirectoryScannerDll, "ScanForFilesInDirectory");
  if (!scanDirectoryFunc) {
    logError("Failed to get function address for ScanForFilesInDirectory");
    unloadDll(hDirectoryScannerDll);
    return;
  }

  scanDirectoryFunc(directory.wstring().c_str(), searchOnlyForEncFiles, fileList, callback);

  unloadDll(hDirectoryScannerDll);
}