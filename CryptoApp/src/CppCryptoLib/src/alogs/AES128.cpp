#include "AES128.h"

/**
 * Encrypts a file using AES-128-CBC encryption.
 *
 * @param fileData Pointer to the FileData object containing file paths, key, and IV.
 * @return True if the encryption is successful, false otherwise.
 */
bool AES128::encryptFile(const FileData *fileData) {
  if (!fileData || fileData->Key->empty() <= 0 || fileData->Iv->empty() ) {
    std::cerr << "AES128: Null pointer in FileData" << std::endl;
    return false;
  }

  fs::create_directories(fileData->EncryptedFilePath->parent_path());

  std::ifstream infile(fileData->OriginalFilePath->string(), std::ios::binary);
  if (!infile.is_open()) {
    std::wcerr << L"Failed to open input file: " << fileData->OriginalFilePath << std::endl;
    return false;
  }

  std::ofstream outfile(fileData->EncryptedFilePath->string(), std::ios::binary);
  if (!outfile.is_open()) {
    std::wcerr << L"Failed to open output file: " << fileData->EncryptedFilePath->wstring() << std::endl;
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

  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, fileData->Key->data(), fileData->Iv->data())) {
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

  CryptoHelperUtils::deleteFile(fileData->OriginalFilePath->wstring());

  if (!CryptoHelperUtils::MarkFile(fileData)) {
    return false;
  }

  return true;
}

/**
 * Decrypts a file using AES-128-CBC decryption.
 *
 * @param fileData Pointer to the FileData object containing file paths, key, and IV.
 * @return True if the decryption is successful, false otherwise.
 */
bool AES128::decryptFile(const FileData *fileData) {
  if (!fileData || fileData->Key->empty() <= 0 || fileData->Iv->empty() ) {
    std::cerr << "AES128: Null pointer in FileData" << std::endl;
    return false;
  }

  if (!CryptoHelperUtils::UnmarkFile(fileData)) {
    std::wcerr << L"Failed to unmark file: " << fileData->EncryptedFilePath->wstring() << std::endl;
    return false;
  }

  fs::create_directories(fileData->DecryptedFilePath->parent_path());

  std::ifstream infile(fileData->EncryptedFilePath->string(), std::ios::binary);
  if (!infile.is_open()) {
    std::wcerr << L"Failed to open input file: " << fileData->EncryptedFilePath->wstring() << std::endl;
    return false;
  }

  std::ofstream outfile(fileData->DecryptedFilePath->string(), std::ios::binary);
  if (!outfile.is_open()) {
    std::wcerr << L"Failed to open output file: " << fileData->DecryptedFilePath->wstring() << std::endl;
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

  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, fileData->Key->data(), fileData->Iv->data())) {
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

  CryptoHelperUtils::deleteFile(fileData->EncryptedFilePath->wstring());

  return true;
}