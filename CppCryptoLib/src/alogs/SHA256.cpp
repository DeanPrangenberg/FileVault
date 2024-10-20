#include <openssl/evp.h>
#include "../CryptoNamespace.h"

using easyOpenSSL::SHA256;

void SHA256::hashFile(const wchar_t *filePath, unsigned char **fileID, int *fileIDLength) {
  std::wstring filePathWStr(filePath);
  std::string filePathUtf8(filePathWStr.begin(), filePathWStr.end());

  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int hashLength = 0;

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (mdctx == nullptr) {
    throw std::runtime_error("Failed to create EVP_MD_CTX");
  }

  if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to initialize digest");
  }

  if (EVP_DigestUpdate(mdctx, filePathUtf8.c_str(), filePathUtf8.size()) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to update digest");
  }

  if (EVP_DigestFinal_ex(mdctx, hash, &hashLength) != 1) {
    EVP_MD_CTX_free(mdctx);
    throw std::runtime_error("Failed to finalize digest");
  }

  EVP_MD_CTX_free(mdctx);

  *fileID = new unsigned char[hashLength];
  std::copy(hash, hash + hashLength, *fileID);
  *fileIDLength = hashLength;
}