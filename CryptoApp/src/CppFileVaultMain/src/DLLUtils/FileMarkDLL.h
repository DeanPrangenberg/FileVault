//
// Created by prang on 20.11.2024.
//

#ifndef FILEVAULTROOT_FILEMARKDLL_H
#define FILEVAULTROOT_FILEMARKDLL_H

#include "MasterDLLClass.h"

class FileMarkDLL : MasterDLLClass {
public:
  bool extractIDsFromFile(const wchar_t * filePath, unsigned char FileID[64], unsigned char EncryptionID[64]);

private:
  typedef bool (*ExtractIDsFromFileFunc)(const wchar_t *, unsigned char[64], unsigned char[64]);
};


#endif //FILEVAULTROOT_FILEMARKDLL_H
