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
  std::unordered_map<std::string, bool> ReadBoolsFromSettings();
  fs::path GetStorageLocation();
  std::vector<unsigned char> GetPasswordHash();

private:
  const std::string SETTINGS_FILE = "settings.json";
};


#endif //FILEVAULTROOT_GETSETTINGSDATA_H
