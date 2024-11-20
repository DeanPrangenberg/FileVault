//
// Created by prang on 20.11.2024.
//

#ifndef FILEVAULTROOT_MULTICRYPTODLL_H
#define FILEVAULTROOT_MULTICRYPTODLL_H

#include "MasterDLLClass.h"

class MultiCryptoDLL : MasterDLLClass {
public:
  void EncryptFiles(struct FileData *fileData, int count, std::vector<bool> &results);
  void DecryptFiles(struct FileData *fileData, int count, std::vector<bool> &results);

private:
  typedef bool* (*EncryptFilesFunc)(struct FileData *, int);
  typedef bool* (*DecryptFilesFunc)(struct FileData *, int);
};


#endif //FILEVAULTROOT_MULTICRYPTODLL_H
