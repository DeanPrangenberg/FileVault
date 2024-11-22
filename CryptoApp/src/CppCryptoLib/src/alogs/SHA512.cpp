#include "SHA512.h"

std::array<unsigned char, EVP_MAX_MD_SIZE> SHA512::hashFile(const wchar_t *filePath) {
    std::ifstream fileStream(filePath, std::ios::binary);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    std::array<unsigned char, EVP_MAX_MD_SIZE> hash {NULL};
    unsigned int hashLength = 0;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha512(), nullptr) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to initialize digest");
    }

    char buffer[4096];
    while (fileStream.read(buffer, sizeof(buffer)) || fileStream.gcount() > 0) {
        if (EVP_DigestUpdate(mdctx, buffer, fileStream.gcount()) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to update digest");
        }
    }

    if (EVP_DigestFinal_ex(mdctx, hash.data(), &hashLength) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to finalize digest");
    }

    EVP_MD_CTX_free(mdctx);

    return hash;
}

std::array<unsigned char, EVP_MAX_MD_SIZE> SHA512::hashString(const std::wstring &input) {
  std::array<unsigned char, EVP_MAX_MD_SIZE> hash {NULL};
  unsigned int hashLength = 0;

  // Convert std::wstring to std::string (UTF-8 encoding)
  std::string utf8Input(input.begin(), input.end());

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (mdctx == nullptr) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }

  if (EVP_DigestInit_ex(mdctx, EVP_sha512(), nullptr) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to initialize digest");
  }

  if (EVP_DigestUpdate(mdctx, utf8Input.data(), utf8Input.size()) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to update digest");
  }

  if (EVP_DigestFinal_ex(mdctx, hash.data(), &hashLength) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to finalize digest");
  }

  EVP_MD_CTX_free(mdctx);

  return hash;
}