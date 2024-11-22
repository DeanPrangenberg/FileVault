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
#include "../../../CppFileVaultMain/src/GlobalDefinitions.h"
#include "../Helper/HelperUtils.h"

using globalDefinitions::FileData;
namespace fs = std::filesystem;

class AES256 {
    public:
    static bool encryptFile(const FileData *fileData);
    static bool decryptFile(const FileData *fileData);
};

#endif //FILEVAULTROOT_AES256_H
