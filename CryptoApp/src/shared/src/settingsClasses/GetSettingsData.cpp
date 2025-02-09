#include <sstream>
#include "GetSettingsData.h"

/**
 * @brief Reads boolean settings from a JSON file.
 *
 * @return A map containing the boolean settings.
 */
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

/**
 * @brief Gets the storage location from the settings file.
 *
 * @return The storage location path.
 */
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

/**
 * @brief Gets the password hash from the settings file.
 *
 * @return A vector containing the password hash.
 */
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

/**
 * @brief Gets the salt from the settings file.
 *
 * @return A vector containing the salt.
 */
std::vector<unsigned char> GetSettingsData::GetSalt() {
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
  std::cout << "Salt hash: " << jsonData["Salt"] << std::endl;
  if (jsonData["Salt"].is_null() || jsonData["Salt"].empty()) {
    return std::vector<unsigned char>();
  }
  std::string SaltHex = jsonData["Salt"];

  if (SaltHex.size() <= 0) {
    std::cerr << "Error: Password hash has invalid length!" << std::endl;
    return std::vector<unsigned char>();
  }

  std::vector<unsigned char> hashVector;
  for (size_t i = 0; i < SaltHex.size(); i += 2) {
    std::string byteString = SaltHex.substr(i, 2);
    unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
    hashVector.push_back(byte);
  }
  std::cout << "Password hash successfully read from file" << std::endl;
  return hashVector;
}

/**
 * @brief Sets the password hash in the settings file.
 *
 * @param hashVector A vector containing the password hash.
 * @return True if the password hash was successfully set, false otherwise.
 */
bool GetSettingsData::SetPasswordHash(const std::vector<unsigned char>& hashVector) {
  if (hashVector.size() != 64) {
    std::cerr << "Error: Password hash must be 64 bytes (128 hex chars)!\n";
    return false;
  }

  // Byte-Vektor zu Hex-String konvertieren
  std::stringstream hexStream;
  hexStream << std::hex << std::setfill('0');
  for (const auto& byte : hashVector) {
    hexStream << std::setw(2) << static_cast<int>(byte);
  }
  std::string passwordHex = hexStream.str();

  // JSON-Daten lesen oder neu erstellen
  json jsonData;
  std::ifstream inFile(SETTINGS_FILE);
  if (inFile.is_open()) {
    inFile >> jsonData;
    inFile.close();
  }

  // Passwort aktualisieren
  jsonData["Password"] = passwordHex;

  // In Datei schreiben
  std::ofstream outFile(SETTINGS_FILE);
  if (!outFile.is_open()) {
    std::cerr << "Error: Could not write to settings file!\n";
    return false;
  }
  outFile << jsonData.dump(4);  // Mit Formatierung
  return true;
}

/**
 * @brief Sets the salt in the settings file.
 *
 * @param saltVector A vector containing the salt.
 * @return True if the salt was successfully set, false otherwise.
 */
bool GetSettingsData::SetSalt(const std::vector<unsigned char>& saltVector) {
  if (saltVector.size() != 16) {
    std::cerr << "Error: Salt must be 16 bytes!\n";
    return false;
  }

  // Byte-Vektor zu Hex-String konvertieren
  std::stringstream hexStream;
  hexStream << std::hex << std::setfill('0');
  for (const auto& byte : saltVector) {
    hexStream << std::setw(2) << static_cast<int>(byte);
  }
  std::string saltHex = hexStream.str();

  // JSON-Daten lesen oder neu erstellen
  json jsonData;
  std::ifstream inFile(SETTINGS_FILE);
  if (inFile.is_open()) {
    inFile >> jsonData;
    inFile.close();
  }

  // Salt aktualisieren
  jsonData["Salt"] = saltHex;

  // In Datei schreiben
  std::ofstream outFile(SETTINGS_FILE);
  if (!outFile.is_open()) {
    std::cerr << "Error: Could not write to settings file!\n";
    return false;
  }
  outFile << jsonData.dump(4);  // Mit Formatierung
  return true;
}