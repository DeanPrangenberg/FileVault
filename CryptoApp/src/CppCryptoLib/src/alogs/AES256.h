//
// Created by prang on 23.11.2024.
//

#ifndef FILEVAULTROOT_AES256_H
#define FILEVAULTROOT_AES256_H

#include <openssl/err.h>
#include <openssl/evp.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <vector>
#include <algorithm>
#include "../../../CppFileVaultMain/src/GlobalDefinitions.h"
#include "../Helper/HelperUtils.h"
#undef min
#undef max

namespace fs = std::filesystem;

class AES256 {
    public:
    static bool encryptFile(const FileData *fileData);
    static bool decryptFile(const FileData *fileData);
private:
  bool printDebug = false;
};

#endif //FILEVAULTROOT_AES256_H
