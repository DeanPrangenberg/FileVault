//
// Created by prang on 27.01.2025.
//

#ifndef FILEVAULTROOT_GETSETTINGSDATA_H
#define FILEVAULTROOT_GETSETTINGSDATA_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

class GetSettingsData {
public:
  /**
   * @brief Reads boolean settings from a JSON file.
   * @return A map containing the boolean settings.
   */
  std::unordered_map<std::string, bool> ReadBoolsFromSettings();

  /**
   * @brief Gets the storage location from the settings file.
   * @return The storage location path.
   */
  fs::path GetStorageLocation();

  /**
   * @brief Gets the password hash from the settings file.
   * @return A vector containing the password hash.
   */
  std::vector<unsigned char> GetPasswordHash();

  /**
   * @brief Gets the salt from the settings file.
   * @return A vector containing the salt.
   */
  std::vector<unsigned char> GetSalt();

  /**
   * @brief Sets the salt in the settings file.
   * @param saltVector A vector containing the salt.
   * @return True if the salt was successfully set, false otherwise.
   */
  bool SetSalt(const std::vector<unsigned char> &saltVector);

  /**
   * @brief Sets the password hash in the settings file.
   * @param hashVector A vector containing the password hash.
   * @return True if the password hash was successfully set, false otherwise.
   */
  bool SetPasswordHash(const std::vector<unsigned char> &hashVector);

private:
  /**
   * @brief The path to the settings file.
   */
  const std::string SETTINGS_FILE = "settings.json";
};

#endif //FILEVAULTROOT_GETSETTINGSDATA_H