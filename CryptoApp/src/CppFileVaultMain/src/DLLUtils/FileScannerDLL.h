#ifndef FILEVAULTROOT_FILESCANNERDLL_H
#define FILEVAULTROOT_FILESCANNERDLL_H

#include "MasterDLLClass.h"
#include <functional>
#include <vector>
#include <filesystem>

class FileScannerDLL : MasterDLLClass {
public:
  using FileFoundCallback = std::function<void(const std::filesystem::path&)>;

  void ScanDirectory(const std::filesystem::path& directory, const bool searchOnlyForEncFiles, std::vector<std::filesystem::path> &fileList, FileFoundCallback callback);

private:
  typedef void (*ScanDirectoryFunc)(const wchar_t *, const bool, std::vector<std::filesystem::path> &, FileFoundCallback);
};

#endif //FILEVAULTROOT_FILESCANNERDLL_H