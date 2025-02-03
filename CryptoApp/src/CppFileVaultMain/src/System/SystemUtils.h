//
// Created by prang on 25.09.2024.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

/**
 * @class SystemUtils
 * @brief Utility class for system-related operations.
 */
class SystemUtils {
public:
  /**
   * @brief Retrieves the Windows folder path.
   * @return The path to the Windows folder.
   */
  static fs::path getWindowsFolder();

  /**
   * @brief Retrieves all logical drives on the system.
   * @return A vector of paths representing all logical drives.
   */
  static std::vector<fs::path> getAllDrives();
};

#endif // UTILS_H