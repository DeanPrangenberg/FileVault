#include "FileScannerDLL.h"

/**
 * @brief Scans a directory for files using the CppDirectoryScannerLib DLL.
 *
 * @param directory The path of the directory to scan.
 * @param searchOnlyForEncFiles Flag to indicate whether to search only for encrypted files.
 * @param fileList A vector to store the list of found files.
 * @param callback The callback function to call when a file is found.
 */
void FileScannerDLL::ScanDirectory(const std::filesystem::path& directory, const bool searchOnlyForEncFiles, std::vector<std::filesystem::path> &fileList, FileFoundCallback callback) {
  HMODULE hDirectoryScannerDll = loadDll(L"CppDirectoryScannerLib.dll");
  if (!hDirectoryScannerDll) {
    return;
  }

  auto scanDirectoryFunc = (ScanDirectoryFunc) GetProcAddress(hDirectoryScannerDll, "ScanForFilesInDirectory");
  if (!scanDirectoryFunc) {
    Logs::writeToErrorLog("Failed to get function address for ScanForFilesInDirectory");
    unloadDll(hDirectoryScannerDll);
    return;
  }

  scanDirectoryFunc(directory.wstring().c_str(), searchOnlyForEncFiles, fileList, callback);

  unloadDll(hDirectoryScannerDll);
}