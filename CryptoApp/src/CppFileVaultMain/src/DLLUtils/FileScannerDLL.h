//
// Created by prang on 20.11.2024.
//

#ifndef FILEVAULTROOT_FILESCANNERDLL_H
#define FILEVAULTROOT_FILESCANNERDLL_H

#include "MasterDLLClass.h"

class FileScannerDLL : MasterDLLClass {
public:
  std::vector<fs::path> ScanDirectory(const fs::path& directory, const bool searchOnlyForDecryptedFiles);

private:
  typedef const wchar_t ** (*ScanDirectoryFunc)(const wchar_t *, const bool);
};


#endif //FILEVAULTROOT_FILESCANNERDLL_H
