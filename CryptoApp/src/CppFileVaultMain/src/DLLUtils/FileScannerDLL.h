#ifndef FILEVAULTROOT_FILESCANNERDLL_H
#define FILEVAULTROOT_FILESCANNERDLL_H

#include "MasterDLLClass.h"
#include <functional>
#include <vector>
#include <filesystem>

class FileScannerDLL : MasterDLLClass {
public:
  /**
   * @brief Callback type for when a file is found during the directory scan.
   *
   * @param path The path of the found file.
   */
  using FileFoundCallback = std::function<void(const std::filesystem::path&)>;

  /**
   * @brief Scans a directory for files using the CppDirectoryScannerLib DLL.
   *
   * @param directory The path of the directory to scan.
   * @param searchOnlyForEncFiles Flag to indicate whether to search only for encrypted files.
   * @param fileList A vector to store the list of found files.
   * @param callback The callback function to call when a file is found.
   */
  void ScanDirectory(const std::filesystem::path& directory, const bool searchOnlyForEncFiles, std::vector<std::filesystem::path> &fileList, FileFoundCallback callback);

private:
  /**
   * @brief Function pointer type for scanning a directory for files.
   *
   * @param directory The path of the directory to scan.
   * @param searchOnlyForEncFiles Flag to indicate whether to search only for encrypted files.
   * @param fileList A vector to store the list of found files.
   * @param callback The callback function to call when a file is found.
   */
  typedef void (*ScanDirectoryFunc)(const wchar_t *, const bool, std::vector<std::filesystem::path> &, FileFoundCallback);
};

#endif //FILEVAULTROOT_FILESCANNERDLL_H