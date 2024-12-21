// AES256.h
#ifndef FILEVAULTROOT_AES256_H
#define FILEVAULTROOT_AES256_H

#include <openssl/err.h>
#include <openssl/evp.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <future>
#include <mutex>
#include "../../../CppFileVaultMain/src/GlobalDefinitions.h"
#include "../Helper/HelperUtils.h"
#undef min

namespace fs = std::filesystem;

class AES256 {
public:
  static bool encryptFile(const FileData *fileData);
  static bool decryptFile(const FileData *fileData);
private:
  static std::mutex fileMutex;
};

#endif //FILEVAULTROOT_AES256_H