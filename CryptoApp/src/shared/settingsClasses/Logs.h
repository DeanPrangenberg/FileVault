#ifndef LOGS_H
#define LOGS_H

#include <filesystem>
#include <fstream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>

namespace fs = std::filesystem;

class Logs {
public:
  static void writeToErrorLog(const std::string &message);
  static void writeToInfoLog(const std::string &message);
  static void moveLogsDirectory(const fs::path &newLogsDirectory);
  static fs::path getLogsDirectory();

private:
  static fs::path logsDirectory;
  static bool checkFileExists(const fs::path &filePath);
  static std::string getCurrentTimeFormatted();
  static void ensureLogsDirectoryExists(const fs::path &logsDir);
};

#endif //LOGS_H