//
// Created by prang on 09.10.2024.
//

#include "../CryptoNamespace.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <iostream>
#include <fstream>

using easyOpenSSL::AES128;

void AES128::printError(const std::string &msg) {
  std::cerr << msg << " Error: " << ERR_reason_error_string(ERR_get_error()) << std::endl;
}

bool AES128::encryptFile(
    const fs::path &originalFile,
    const fs::path &encryptedFile,
    const std::vector<unsigned char> &KEY,
    const std::vector<unsigned char> &IV
) {

  // Initialisiere den Verschlüsselungskontext
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    std::cerr << "Failed to create EVP_CIPHER_CTX." << std::endl;
    return false;
  }

  // Setze den AES-128-CBC Algorithmus mit Schlüssel und IV
  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, KEY.data(), IV.data())) {
    printError("EVP_EncryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Öffne die Eingabedatei und die Ausgabedatei
  std::ifstream infile(originalFile, std::ios::binary);
  std::ofstream outfile(encryptedFile, std::ios::binary);
  if (!infile.is_open()) {
    std::wcerr << L"Failed to open input file: " << originalFile.wstring() << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::wcerr << L"Failed to open output file: " << encryptedFile.wstring() << std::endl;
    infile.close();
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  const size_t buffer_size = 16384; // Erhöhte Puffergröße auf 16384 Bytes
  unsigned char buffer[buffer_size];
  unsigned char ciphertext[buffer_size + EVP_MAX_BLOCK_LENGTH]; // Platz für Padding
  int len;

  // Datei blockweise verschlüsseln
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

  // Finalisiere die Verschlüsselung und füge Padding hinzu
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

  // Freigeben des Kontextes
  EVP_CIPHER_CTX_free(ctx);

  return true;
}

bool AES128::decryptFile(
    const fs::path &encryptedFile,
    const fs::path &decryptedFile,
    const std::vector<unsigned char> &KEY,
    const std::vector<unsigned char> &IV
) {

  // Initialisiere den Entschlüsselungskontext
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    std::cerr << "Failed to create EVP_CIPHER_CTX." << std::endl;
    return false;
  }

  // Setze den AES-128-CBC Algorithmus mit Schlüssel und IV
  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, KEY.data(), IV.data())) {
    printError("EVP_DecryptInit_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Öffne die verschlüsselte Datei und die Ausgabedatei für die Entschlüsselung
  std::ifstream infile(encryptedFile, std::ios::binary);
  std::ofstream outfile(decryptedFile, std::ios::binary);
  if (!infile.is_open()) {
    std::cerr << "Failed to open encrypted input file: " << encryptedFile.string() << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::cerr << "Failed to open output file: " << decryptedFile.string() << std::endl;
    infile.close();
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  const size_t buffer_size = 16384; // Erhöhte Puffergröße auf 16384 Bytes
  unsigned char buffer[buffer_size];
  unsigned char plaintext[buffer_size + EVP_MAX_BLOCK_LENGTH];
  int len;

  // Datei blockweise entschlüsseln
  while (infile.read(reinterpret_cast<char *>(buffer), buffer_size) || infile.gcount() > 0) {
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, buffer, infile.gcount())) {
      printError("EVP_DecryptUpdate failed");
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
    outfile.write(reinterpret_cast<char *>(plaintext), len);
    if (outfile.fail()) {
      std::cerr << "Failed to write plaintext to output file." << std::endl;
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }
  }

  // Finalisiere die Entschlüsselung und überprüfe das Padding
  if (1 != EVP_DecryptFinal_ex(ctx, plaintext, &len)) {
    printError("EVP_DecryptFinal_ex failed");
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  outfile.write(reinterpret_cast<char *>(plaintext), len);
  if (outfile.fail()) {
    std::cerr << "Failed to write final plaintext block to output file." << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Freigeben des Kontextes
  EVP_CIPHER_CTX_free(ctx);
  return true;
}
