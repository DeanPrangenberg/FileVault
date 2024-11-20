//
// Created by prang on 20.11.2024.
//

#ifndef FILEVAULTROOT_CRYPTODLL_H
#define FILEVAULTROOT_CRYPTODLL_H

#include "MasterDLLClass.h"

class CryptoDLL : MasterDLLClass  {
public:
  void GenerateKeyIv(size_t size, unsigned char *key, unsigned char *iv);
  void GenerateFileID(const wchar_t *filePath, unsigned char *fileID);

private:
  typedef void (*GenerateKeyIvFunc)(size_t, unsigned char *, unsigned char *);
  typedef void (*GenerateFileIDFunc)(const wchar_t *, unsigned char *);
};


#endif //FILEVAULTROOT_CRYPTODLL_H
