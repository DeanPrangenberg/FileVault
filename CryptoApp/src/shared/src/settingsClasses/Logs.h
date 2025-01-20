// Logs.h
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
  /**
   * @brief Gets the singleton instance of Logs.
   * @return The singleton instance.
   */
  static Logs& getInstance() {
    static Logs instance;
    return instance;
  }

  // Delete copy constructor and assignment operator to prevent copies
  Logs(const Logs&) = delete;
  void operator=(const Logs&) = delete;

  /**
   * @brief Writes a message to the error log.
   *
   * @param message The error message to log.
   */
  static void writeToErrorLog(const std::string &message);

  /**
   * @brief Writes a message to the info log.
   *
   * @param message The info message to log.
   */
  static void writeToInfoLog(const std::string &message);

  /**
   * @brief Moves the logs directory to a new location.
   *
   * @param newLogsDirectory The new path for the logs directory.
   */
  static void moveLogsDirectory(const fs::path &newLogsDirectory);

  /**
   * @brief Gets the current logs directory path.
   *
   * @return The path to the logs directory.
   */
  static fs::path getLogsDirectory();

private:
  /**
   * @brief Default constructor for Logs.
   */
  Logs() = default;

  /**
   * @brief Path to the logs directory.
   */
  static fs::path logsDirectory;

  /**
   * @brief Checks if a file exists at the given path.
   *
   * @param filePath The path to the file.
   * @return True if the file exists, false otherwise.
   */
  static bool checkFileExists(const fs::path &filePath);

  /**
   * @brief Gets the current time formatted as a string.
   *
   * @return The current time formatted as a string.
   */
  static std::string getCurrentTimeFormatted();

  /**
   * @brief Ensures that the logs directory exists, creating it if necessary.
   *
   * @param logsDir The path to the logs directory.
   */
  static void ensureLogsDirectoryExists(const fs::path &logsDir);
};

#endif //LOGS_H