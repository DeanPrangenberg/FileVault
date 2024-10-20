//
// Created by prang on 09.10.2024.
//

#include "../CryptoNamespace.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <iostream>
#include <fstream>

using easyOpenSSL::AES256;

void AES256::printError(const std::string &msg) {
  std::cerr << msg << " Error: " << ERR_reason_error_string(ERR_get_error()) << std::endl;
}

bool AES256::encryptFile(
    const fs::path &originalFile,
    const fs::path &encryptedFile,
    const std::vector<unsigned char> &KEY,
    const std::vector<unsigned char> &IV
) {

  // Initialize encryption context
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    printError("Failed to create EVP_CIPHER_CTX");
    return false;
  }

  // Set AES-256-CBC algorithm with key and IV
  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, KEY.data(), IV.data())) {
    printError("EVP_EncryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Open input and output files
  std::ifstream infile(originalFile, std::ios::binary);
  std::ofstream outfile(encryptedFile, std::ios::binary);
  if (!infile.is_open()) {
    std::cerr << "Failed to open input file: " << originalFile.string() << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::cerr << "Failed to open output file: " << encryptedFile.string() << std::endl;
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
      printError("EVP_EncryptUpdate failed");
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
    outfile.write(reinterpret_cast<char *>(ciphertext), len);
    if (outfile.fail()) {
      std::cerr << "Failed to write ciphertext to output file." << std::endl;
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
  }

  // Finalize the encryption and add padding
  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext, &len)) {
    printError("EVP_EncryptFinal_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  outfile.write(reinterpret_cast<char *>(ciphertext), len);
  if (outfile.fail()) {
    std::cerr << "Failed to write final ciphertext block to output file." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Free the context
  EVP_CIPHER_CTX_free(ctx);
  return true;
}

bool AES256::decryptFile(
    const fs::path &encryptedFile,
    const fs::path &decryptedFile,
    const std::vector<unsigned char> &KEY,
    const std::vector<unsigned char> &IV
) {

  // Überprüfen Sie die Schlüssellänge
  if (KEY.size() != 32 || IV.size() != 16) {
    std::cerr << "Invalid key or IV size!" << std::endl;
    return false;
  }

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    printError("Failed to create EVP_CIPHER_CTX");
    return false;
  }

  // Initialisieren Sie den Dekryptierungs-Kontext mit AES-256
  if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, KEY.data(), IV.data()) != 1) {
    printError("EVP_DecryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  std::ifstream infile(encryptedFile, std::ios::binary);
  std::ofstream outfile(decryptedFile, std::ios::binary);
  if (!infile.is_open()) {
    std::cerr << "Failed to open input file: " << encryptedFile << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::cerr << "Failed to open output file: " << decryptedFile << std::endl;
    infile.close();
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  const size_t buffer_size = 16384; // Puffergröße
  unsigned char buffer[buffer_size];
  unsigned char plaintext[buffer_size + EVP_MAX_BLOCK_LENGTH]; // Puffer für entschlüsselte Daten
  int len = 0;

  // Entschlüsseln Sie die Datei blockweise
  while (infile.read(reinterpret_cast<char *>(buffer), buffer_size) || infile.gcount() > 0) {

    // Entschlüsseln Sie den aktuellen Block
    if (EVP_DecryptUpdate(ctx, plaintext, &len, buffer, infile.gcount()) != 1) {
      printError("EVP_DecryptUpdate failed");
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }

    // Schreiben Sie die entschlüsselten Daten in die Ausgabedatei
    if (outfile.write(reinterpret_cast<char *>(plaintext), len).fail()) {
      std::cerr << "Failed to write plaintext to output file." << std::endl;
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
  }

  // Finalisieren Sie die Entschlüsselung
  int finalLen = 0;
  if (EVP_DecryptFinal_ex(ctx, plaintext, &finalLen) != 1) {
    printError("EVP_DecryptFinal_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Schreiben Sie den letzten Block in die Ausgabedatei
  if (outfile.write(reinterpret_cast<char *>(plaintext), finalLen).fail()) {
    std::cerr << "Failed to write final plaintext to output file." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  EVP_CIPHER_CTX_free(ctx);
  return true;
}