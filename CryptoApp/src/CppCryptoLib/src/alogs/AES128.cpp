// AES128.cpp
#include "AES128.h"

std::mutex AES128::fileMutex;

bool AES128::encryptFile(const FileData *fileData) {
  if (!fileData || !fileData->getKey() || !fileData->getIv()) {
    std::cerr << "AES128: Null pointer in FileData" << std::endl;
    return false;
  }

  std::vector<unsigned char> keyVec(fileData->getKey(), fileData->getKey() + fileData->getKeyLength());
  std::vector<unsigned char> ivVec(fileData->getIv(), fileData->getIv() + fileData->getIvLength());

  fs::path originalFile(fileData->getOriginalFilePath());
  fs::path encryptedFile(fileData->getEncryptedFilePath());

  fs::create_directories(encryptedFile.parent_path());

  std::ifstream infile(originalFile, std::ios::binary);
  if (!infile.is_open()) {
    std::wcerr << L"Failed to open input file: " << originalFile.wstring() << std::endl;
    return false;
  }

  std::ofstream outfile(encryptedFile, std::ios::binary);
  if (!outfile.is_open()) {
    std::wcerr << L"Failed to open output file: " << encryptedFile.wstring() << std::endl;
    infile.close();
    return false;
  }

  size_t buffer_size = 65536; // Increased buffer size
  std::vector<unsigned char> buffer(buffer_size);
  std::vector<unsigned char> ciphertext(buffer_size + EVP_MAX_BLOCK_LENGTH);
  int len;

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    std::wcerr << "Failed to create EVP_CIPHER_CTX." << std::endl;
    return false;
  }

  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, keyVec.data(), ivVec.data())) {
    CryptoHelperUtils::printError(L"EVP_EncryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  while (infile.read(reinterpret_cast<char *>(buffer.data()), buffer.size()) || infile.gcount() > 0) {
    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, buffer.data(), infile.gcount())) {
      CryptoHelperUtils::printError(L"EVP_EncryptUpdate failed");
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
    outfile.write(reinterpret_cast<char *>(ciphertext.data()), len);
  }

  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data(), &len)) {
    CryptoHelperUtils::printError(L"EVP_EncryptFinal_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  outfile.write(reinterpret_cast<char *>(ciphertext.data()), len);

  EVP_CIPHER_CTX_free(ctx);
  infile.close();
  outfile.close();

  CryptoHelperUtils::deleteFile(originalFile);
  CryptoHelperUtils::MarkFile(fileData);

  return true;
}

bool AES128::decryptFile(const FileData *fileData) {
  if (!fileData || !fileData->getKey() || !fileData->getIv()) {
    std::cerr << "AES128: Null pointer in FileData" << std::endl;
    return false;
  }

  if (!CryptoHelperUtils::UnmarkFile(fileData)) {
    std::wcerr << L"Failed to unmark file: " << fileData->getEncryptedFilePath() << std::endl;
    return false;
  }

  std::vector<unsigned char> keyVec(fileData->getKey(), fileData->getKey() + fileData->getKeyLength());
  std::vector<unsigned char> ivVec(fileData->getIv(), fileData->getIv() + fileData->getIvLength());

  fs::path encryptedFile(fileData->getEncryptedFilePath());
  fs::path decryptedFile(fileData->getDecryptedFilePath());

  fs::create_directories(decryptedFile.parent_path());

  std::ifstream infile(encryptedFile, std::ios::binary);
  if (!infile.is_open()) {
    std::wcerr << L"Failed to open input file: " << encryptedFile.wstring() << std::endl;
    return false;
  }

  std::ofstream outfile(decryptedFile, std::ios::binary);
  if (!outfile.is_open()) {
    std::wcerr << L"Failed to open output file: " << decryptedFile.wstring() << std::endl;
    infile.close();
    return false;
  }

  size_t buffer_size = 65536; // Increased buffer size
  std::vector<unsigned char> buffer(buffer_size);
  std::vector<unsigned char> plaintext(buffer_size + EVP_MAX_BLOCK_LENGTH);
  int len;

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    std::wcerr << "Failed to create EVP_CIPHER_CTX." << std::endl;
    return false;
  }

  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, keyVec.data(), ivVec.data())) {
    CryptoHelperUtils::printError(L"EVP_DecryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  while (infile.read(reinterpret_cast<char *>(buffer.data()), buffer.size()) || infile.gcount() > 0) {
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, buffer.data(), infile.gcount())) {
      CryptoHelperUtils::printError(L"EVP_DecryptUpdate failed");
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
    outfile.write(reinterpret_cast<char *>(plaintext.data()), len);
  }

  if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data(), &len)) {
    CryptoHelperUtils::printError(L"EVP_DecryptFinal_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  outfile.write(reinterpret_cast<char *>(plaintext.data()), len);

  EVP_CIPHER_CTX_free(ctx);
  infile.close();
  outfile.close();

  CryptoHelperUtils::deleteFile(encryptedFile);

  return true;
}