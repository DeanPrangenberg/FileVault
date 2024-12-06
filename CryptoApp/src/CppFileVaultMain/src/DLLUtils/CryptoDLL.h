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
  void EncryptFiles(class FileData *fileData, int count, std::vector<bool> &results);
  void DecryptFiles(class FileData *fileData, int count, std::vector<bool> &results);

private:
  typedef void (*GenerateKeyIvFunc)(size_t, unsigned char *, unsigned char *);
  typedef void (*GenerateFileIDFunc)(const wchar_t *, unsigned char *);
  typedef bool* (*EncryptFilesFunc)(class FileData *, int);
  typedef bool* (*DecryptFilesFunc)(class FileData *, int);
};


#endif //FILEVAULTROOT_CRYPTODLL_H
