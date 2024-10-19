//
// Created by prang on 25.09.2024.
//

#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class SystemUtils {
public:
  static fs::path getWindowsFolder();

  static std::vector<fs::path> getAllDrives();
};

#endif // UTILS_H
