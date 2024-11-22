//
// Created by prang on 23.11.2024.
//

#ifndef FILEVAULTROOT_SHA512_H
#define FILEVAULTROOT_SHA512_H

#include "../../../CppFileVaultMain/src/GlobalDefinitions.h"
#include <openssl/evp.h>
#include <array>
#include <fstream>
#include <stdexcept>

using globalDefinitions::FileData;

class SHA512 {
    public:
    static std::array<unsigned char, EVP_MAX_MD_SIZE> hashFile(const wchar_t *filePath);
    static std::array<unsigned char, EVP_MAX_MD_SIZE> hashString(const std::wstring &input);
};

#endif //FILEVAULTROOT_SHA512_H