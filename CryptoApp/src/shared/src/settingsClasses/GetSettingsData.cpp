#include "GetSettingsData.h"

std::unordered_map<std::string, bool> GetSettingsData::ReadBoolsFromSettings() {
  std::ifstream file(SETTINGS_FILE);
  json jsonData;

  // Open and parse the JSON file
  if (file.is_open()) {
    file >> jsonData;
    file.close();
  } else {
    std::cerr << "Fehler: JSON-Datei konnte nicht geöffnet werden!" << std::endl;
    return {{"DeleteAfterDecryption", false},
            {"DeleteAfterEncryption", false},
            {"ForDecryptedFiles",     false},
            {"ForEncryptedFiles",     false}};  // Return default values
  }

  // Extract boolean values
  std::cout << "Reading settings from file..." << std::endl;
  std::unordered_map<std::string, bool> boolValues = {
      {"DeleteAfterDecryption", jsonData["DeleteAfterDecryption"]},
      {"DeleteAfterEncryption", jsonData["DeleteAfterEncryption"]},
      {"ForDecryptedFiles",     jsonData["ForDecryptedFiles"]},
      {"ForEncryptedFiles",     jsonData["ForEncryptedFiles"]}
  };

  for (const auto &[key, value]: boolValues) {
    std::cout << key << ": " << value << std::endl;
  }

  return boolValues;
}

fs::path GetSettingsData::GetStorageLocation() {
  std::ifstream file(SETTINGS_FILE);
  json jsonData;

  // Open and parse the JSON file
  if (file.is_open()) {
    file >> jsonData;
    file.close();
  } else {
    std::cerr << "Fehler: JSON-Datei konnte nicht geöffnet werden!" << std::endl;
    return fs::path("centralStorage");  // Return default value
  }

  // Extract storage path
  std::cout << "Reading settings from file..." << std::endl;
  std::cout << "Storage path: " << jsonData["StoragePath"] << std::endl;

  if (jsonData["StoragePath"].is_null() || jsonData["StoragePath"].empty()) {
    return fs::path("centralStorage");
  }

  return fs::path(jsonData["StoragePath"].get<std::string>());
}

std::vector<unsigned char> GetSettingsData::GetPasswordHash() {
  std::ifstream file(SETTINGS_FILE);
  json jsonData;

  if (file.is_open()) {
    file >> jsonData;
    file.close();
  } else {
    std::cerr << "Fehler: JSON-Datei konnte nicht geöffnet werden!" << std::endl;
    return std::vector<unsigned char>();
  }

  std::cout << "Reading settings from file..." << std::endl;
  std::cout << "Password hash: " << jsonData["Password"] << std::endl;
  if (jsonData["Password"].is_null() || jsonData["Password"].empty()) {
    return std::vector<unsigned char>();
  }
  std::string passwordHashHex = jsonData["Password"];

  if (passwordHashHex.size() != 128) {
    std::cerr << "Error: Password hash has invalid length!" << std::endl;
    return std::vector<unsigned char>();
  }

  std::vector<unsigned char> hashVector;
  for (size_t i = 0; i < passwordHashHex.size(); i += 2) {
    std::string byteString = passwordHashHex.substr(i, 2);
    unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
    hashVector.push_back(byte);
  }
  std::cout << "Password hash successfully read from file" << std::endl;
  return hashVector;
}
