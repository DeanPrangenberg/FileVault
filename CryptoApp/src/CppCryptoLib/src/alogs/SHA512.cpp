#include "SHA512.h"

std::array<unsigned char, EVP_MAX_MD_SIZE> SHA512::hashFile(const wchar_t *filePath) {
  std::ifstream fileStream(filePath, std::ios::binary);
  if (!fileStream.is_open()) {
    throw std::runtime_error("Failed to open file");
  }

  std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
  unsigned int hashLength = 0;
  char buffer[4096] = {0}; // Initialisierter Puffer

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }

  try {
    if (EVP_DigestInit_ex(mdctx, EVP_sha512(), nullptr) != 1) {
      throw std::runtime_error("Failed to initialize digest");
    }

    while (fileStream.read(buffer, sizeof(buffer)) || fileStream.gcount() > 0) {
      if (EVP_DigestUpdate(mdctx, buffer, fileStream.gcount()) != 1) {
        throw std::runtime_error("Failed to update digest");
      }
      std::memset(buffer, 0, sizeof(buffer)); // Puffer nach jedem Lesen leeren
    }

    if (EVP_DigestFinal_ex(mdctx, hash.data(), &hashLength) != 1) {
      throw std::runtime_error("Failed to finalize digest");
    }
  } catch (...) {
    EVP_MD_CTX_free(mdctx);
    throw; // Fehler weitergeben
  }

  EVP_MD_CTX_free(mdctx);
  return hash;
}

std::array<unsigned char, EVP_MAX_MD_SIZE> SHA512::hashString(const std::wstring &input) {
  std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
  unsigned int hashLength = 0;

  int size_needed = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, nullptr, 0, nullptr, nullptr);
  if (size_needed <= 0) {
    throw std::runtime_error("Failed to convert string to UTF-8");
  }

  std::string utf8Input(size_needed, '\0');
  WideCharToMultiByte(CP_UTF8, 0, input.c_str(), -1, utf8Input.data(), size_needed, nullptr, nullptr);

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }

  try {
    if (EVP_DigestInit_ex(mdctx, EVP_sha512(), nullptr) != 1) {
      throw std::runtime_error("Failed to initialize digest");
    }

    if (EVP_DigestUpdate(mdctx, utf8Input.data(), utf8Input.size()) != 1) {
      throw std::runtime_error("Failed to update digest");
    }

    if (EVP_DigestFinal_ex(mdctx, hash.data(), &hashLength) != 1) {
      throw std::runtime_error("Failed to finalize digest");
    }
  } catch (...) {
    EVP_MD_CTX_free(mdctx);
    throw;
  }

  EVP_MD_CTX_free(mdctx);
  return hash;
}

std::array<unsigned char, EVP_MAX_MD_SIZE> SHA512::hashArray(const std::vector<unsigned char> &input) {
  std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
  unsigned int hashLength = 0;

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }

  try {
    if (EVP_DigestInit_ex(mdctx, EVP_sha512(), nullptr) != 1) {
      throw std::runtime_error("Failed to initialize digest");
    }

    if (EVP_DigestUpdate(mdctx, input.data(), input.size()) != 1) {
      throw std::runtime_error("Failed to update digest");
    }

    if (EVP_DigestFinal_ex(mdctx, hash.data(), &hashLength) != 1) {
      throw std::runtime_error("Failed to finalize digest");
    }
  } catch (...) {
    EVP_MD_CTX_free(mdctx);
    throw;
  }

  EVP_MD_CTX_free(mdctx);
  return hash;
}
