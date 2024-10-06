//
// Created by prang on 25.09.2024.
//

#include "CryptoAes128.h"
#include "FileUtils.h"
#include "KeyUtils.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <iostream>

bool CryptoAes128::encryptFileAES128(const fs::path &input_file, const fs::path &output_file) {
  FileUtils fileUtils;
  KeyUtils keyUtils;

  // Generiere die Verschlüsselungsdaten
  CryptoData encryptionData;
  encryptionData.encryptedPath = output_file;
  encryptionData.keyIvAES = keyUtils.generateKeyIv(16); // 128-bit KEY/IV

  const auto &KEY = encryptionData.keyIvAES.key;
  const auto &IV = encryptionData.keyIvAES.iv;

  // Initialisiere den Verschlüsselungskontext
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    std::cerr << "Failed to create EVP_CIPHER_CTX." << std::endl;
    return false;
  }

  // Setze den AES-128-CBC Algorithmus mit Schlüssel und IV
  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, KEY.data(), IV.data())) {
    std::cerr << "EVP_EncryptInit_ex failed: " << ERR_error_string(ERR_get_error(), nullptr) << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Öffne die Eingabedatei und die Ausgabedatei
  std::ifstream infile(input_file, std::ios::binary);
  std::ofstream outfile(output_file, std::ios::binary);
  if (!infile.is_open()) {
    std::cerr << "Failed to open input file: " << input_file.string() << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::cerr << "Failed to open output file: " << output_file.string() << std::endl;
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
      std::cerr << "EVP_EncryptUpdate failed: " << ERR_error_string(ERR_get_error(), nullptr) << std::endl;
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
    std::cerr << "EVP_EncryptFinal_ex failed: " << ERR_error_string(ERR_get_error(), nullptr) << std::endl;
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

  // Speichern der Verschlüsselungsdaten (z.B. Schlüssel und IV)
  fileUtils.saveCryptoData(encryptionData);

  return true;
}

bool CryptoAes128::decryptFileAES128(const CryptoData &cryptoData, const fs::path &decryptedFilePath) {
  const auto &encryptedFilePath = cryptoData.encryptedPath;
  const auto &KEY = cryptoData.keyIvAES.key;
  const auto &IV = cryptoData.keyIvAES.iv;

  // Initialisiere den Entschlüsselungskontext
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    std::cerr << "Failed to create EVP_CIPHER_CTX." << std::endl;
    return false;
  }

  // Setze den AES-128-CBC Algorithmus mit Schlüssel und IV
  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, KEY.data(), IV.data())) {
    std::cerr << "EVP_DecryptInit_ex failed: " << ERR_error_string(ERR_get_error(), nullptr) << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }

  // Öffne die verschlüsselte Datei und die Ausgabedatei für die Entschlüsselung
  std::ifstream infile(encryptedFilePath, std::ios::binary);
  std::ofstream outfile(decryptedFilePath, std::ios::binary);
  if (!infile.is_open()) {
    std::cerr << "Failed to open encrypted input file: " << encryptedFilePath.string() << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::cerr << "Failed to open output file: " << decryptedFilePath.string() << std::endl;
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
      std::cerr << "EVP_DecryptUpdate failed: " << ERR_error_string(ERR_get_error(), nullptr) << std::endl;
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
    std::cerr << "EVP_DecryptFinal_ex failed: " << ERR_error_string(ERR_get_error(), nullptr) << std::endl;
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
