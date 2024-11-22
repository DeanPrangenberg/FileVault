//
// Created by prang on 09.10.2024.
//

#include "AES256.h"

bool AES256::encryptFile(const FileData *fileData) {
  std::vector<unsigned char> keyVec(fileData->Key, fileData->Key + fileData->keyLength);
  std::vector<unsigned char> ivVec(fileData->Iv, fileData->Iv + fileData->ivLength);

  fs::path originalFile(fileData->OriginalFilePath);
  fs::path encryptedFile(fileData->EncryptedFilePath);

  // Create directories if they do not exist
  fs::create_directories(encryptedFile.parent_path());

  // Initialize encryption context
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    HelperUtils::printError(L"AES256: Failed to create EVP_CIPHER_CTX");
    return false;
  }

  // Set AES-256-CBC algorithm with key and IV
  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, keyVec.data(), ivVec.data())) {
    HelperUtils::printError(L"EVP_EncryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Open input and output files
  std::ifstream infile(originalFile, std::ios::binary);
  std::ofstream outfile(encryptedFile, std::ios::binary);

  if (!infile.is_open()) {
    std::wcerr << L"AES256: Failed to open input file: " << originalFile.wstring() << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::wcerr << "AES256: Failed to open output file: " << encryptedFile.wstring() << std::endl;
    infile.close();
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  const size_t buffer_size = 16384; // Increased buffer size to 16384 bytes
  unsigned char buffer[buffer_size];
  unsigned char ciphertext[buffer_size + EVP_MAX_BLOCK_LENGTH]; // Space for padding
  int len;

  // Encrypt the file block by block
  while (infile.read(reinterpret_cast<char *>(buffer), buffer_size) || infile.gcount() > 0) {
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, buffer, infile.gcount())) {
      HelperUtils::printError(L"EVP_EncryptUpdate failed");
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
    outfile.write(reinterpret_cast<char *>(ciphertext), len);
    if (outfile.fail()) {
      std::wcerr << "AES256: Failed to write ciphertext to output file." << std::endl;
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
  }

  // Finalize the encryption and add padding
  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext, &len)) {
    HelperUtils::printError(L"EVP_EncryptFinal_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  outfile.write(reinterpret_cast<char *>(ciphertext), len);
  if (outfile.fail()) {
    std::wcerr << "AES256: Failed to write final ciphertext block to output file." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Close the file streams
  infile.close();
  outfile.close();

  // Free the context
  EVP_CIPHER_CTX_free(ctx);

  HelperUtils::deleteFile(originalFile);
  HelperUtils::MarkFile(fileData);

  return true;
}

bool AES256::decryptFile(const FileData *fileData) {

  if (!HelperUtils::UnmarkFile(fileData)) {
    std::cerr << "Unmarking failed" << std::endl;
    return false;
  }

  std::vector<unsigned char> keyVec(fileData->Key, fileData->Key + fileData->keyLength);
  std::vector<unsigned char> ivVec(fileData->Iv, fileData->Iv + fileData->ivLength);

  fs::path encryptedFile(fileData->EncryptedFilePath);
  fs::path decryptedFile(fileData->DecryptedFilePath);

  // Create directories if they do not exist
  fs::create_directories(decryptedFile.parent_path());

  // Initialize decryption context
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    HelperUtils::printError(L"AES256: Failed to create EVP_CIPHER_CTX");
    return false;
  }

  // Set AES-256-CBC algorithm with key and IV
  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, keyVec.data(), ivVec.data())) {
    HelperUtils::printError(L"EVP_DecryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Open input and output files
  std::ifstream infile(encryptedFile, std::ios::binary);
  std::ofstream outfile(decryptedFile, std::ios::binary);
  if (!infile.is_open()) {
    std::wcerr << "AES256: Failed to open input file: " << encryptedFile << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::wcerr << "AES256: Failed to open output file: " << decryptedFile << std::endl;
    infile.close();
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  const size_t buffer_size = 16384; // Increased buffer size to 16384 bytes
  unsigned char buffer[buffer_size];
  unsigned char plaintext[buffer_size + EVP_MAX_BLOCK_LENGTH]; // Space for padding
  int len;

  // Decrypt the file block by block
  while (infile.read(reinterpret_cast<char *>(buffer), buffer_size) || infile.gcount() > 0) {
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, buffer, infile.gcount())) {
      HelperUtils::printError(L"EVP_DecryptUpdate failed");
      infile.close();
      outfile.close();
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
    outfile.write(reinterpret_cast<char *>(plaintext), len);
    if (outfile.fail()) {
      std::wcerr << "AES256: Failed to write plaintext to output file." << std::endl;
      infile.close();
      outfile.close();
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
  }

  // Finalize the decryption and add padding
  if (1 != EVP_DecryptFinal_ex(ctx, plaintext, &len)) {
    HelperUtils::printError(L"EVP_DecryptFinal_ex failed");
    infile.close();
    outfile.close();
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  outfile.write(reinterpret_cast<char *>(plaintext), len);
  if (outfile.fail()) {
    std::wcerr << "AES256: Failed to write final plaintext block to output file." << std::endl;
    infile.close();
    outfile.close();
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Close the file streams
  infile.close();
  outfile.close();

  // Free the context
  EVP_CIPHER_CTX_free(ctx);

  HelperUtils::deleteFile(encryptedFile);

  return true;
}