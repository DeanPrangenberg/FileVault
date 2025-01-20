#include "BLAKE2.h"
#include <openssl/evp.h>
#include <windows.h>

/**
 * Computes the BLAKE2 hash of a file.
 *
 * @param filePath The path to the file to be hashed.
 * @return A std::array containing the hash value.
 * @throws std::runtime_error if the file cannot be opened or if any OpenSSL function fails.
 */
std::array<unsigned char, EVP_MAX_MD_SIZE> BLAKE2::hashFile(const wchar_t *filePath) {
  std::ifstream fileStream(filePath, std::ios::binary);
  if (!fileStream.is_open()) {
    throw std::runtime_error("Failed to open file");
  }

  std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
  unsigned int hashLength = 0;
  char buffer[4096] = {0}; // Initialized buffer

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }

  try {
    if (EVP_DigestInit_ex(mdctx, EVP_blake2b512(), nullptr) != 1) {
      throw std::runtime_error("Failed to initialize digest");
    }

    while (fileStream.read(buffer, sizeof(buffer)) || fileStream.gcount() > 0) {
      if (EVP_DigestUpdate(mdctx, buffer, fileStream.gcount()) != 1) {
        throw std::runtime_error("Failed to update digest");
      }
      std::memset(buffer, 0, sizeof(buffer)); // Clear buffer after each read
    }

    if (EVP_DigestFinal_ex(mdctx, hash.data(), &hashLength) != 1) {
      throw std::runtime_error("Failed to finalize digest");
    }
  } catch (...) {
    EVP_MD_CTX_free(mdctx);
    throw; // Rethrow the exception
  }

  EVP_MD_CTX_free(mdctx);
  return hash;
}

/**
 * Computes the BLAKE2 hash of a wide string.
 *
 * @param input The wide string to be hashed.
 * @return A std::array containing the hash value.
 * @throws std::runtime_error if the string cannot be converted to UTF-8 or if any OpenSSL function fails.
 */
std::array<unsigned char, EVP_MAX_MD_SIZE> BLAKE2::hashString(const std::wstring &input) {
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
    if (EVP_DigestInit_ex(mdctx, EVP_blake2b512(), nullptr) != 1) {
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

/**
 * Computes the BLAKE2 hash of a byte array.
 *
 * @param input The byte array to be hashed.
 * @return A std::array containing the hash value.
 * @throws std::runtime_error if any OpenSSL function fails.
 */
std::array<unsigned char, EVP_MAX_MD_SIZE> BLAKE2::hashArray(const std::vector<unsigned char> &input) {
  std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
  unsigned int hashLength = 0;

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }

  try {
    if (EVP_DigestInit_ex(mdctx, EVP_blake2b512(), nullptr) != 1) {
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