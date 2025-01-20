#ifndef FILEVAULTROOT_FILEMARK_H
#define FILEVAULTROOT_FILEMARK_H

#include "../shared/src/GlobalDefinitions.h"
#include "../shared/src/FileData.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
#include <iomanip>

#ifdef FILEMARKLIB_EXPORTS
#define FILEMARKLIB_API __declspec(dllexport)
#else
#define FILEMARKLIB_API __declspec(dllimport)
#endif

extern "C" {
  FILEMARKLIB_API bool extractIDsFromFile(const fs::path *encryptedFilePath, std::vector<unsigned char> *FileID, std::vector<unsigned char> *EncryptionID);
  FILEMARKLIB_API bool markFile(const FileData *fileData);
  FILEMARKLIB_API bool unmarkFile(const FileData *fileData);
}

class FileMarker {
public:
  static bool extractIDsFromFile(const fs::path *encryptedFilePath, std::vector<unsigned char> &FileID, std::vector<unsigned char> &EncryptionID);
  static bool markFile(const FileData *fileData);
  static bool unmarkFile(const FileData *fileData);

private:
  static bool printDebug;
  static std::string toHexString(const unsigned char *pUChar, size_t length);
  static std::vector<unsigned char> fromHexString(const std::string &hexStr);
};

#endif //FILEVAULTROOT_FILEMARK_H