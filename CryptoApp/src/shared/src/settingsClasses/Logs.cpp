#include "Logs.h"

fs::path Logs::logsDirectory = fs::current_path() / "logs";

void Logs::ensureLogsDirectoryExists(const fs::path &logsDir) {
  if (!fs::exists(logsDir)) {
    fs::create_directories(logsDir);
  }
}

void Logs::writeToErrorLog(const std::string &message) {
  ensureLogsDirectoryExists(logsDirectory);
  if (!checkFileExists(logsDirectory / "errorLog.txt")) {
    std::ofstream errorLog(logsDirectory / "errorLog.txt");
    errorLog << "Error Log: " << std::endl;
    errorLog.close();
  }
  std::cerr << getCurrentTimeFormatted() << message << std::endl;
  std::ofstream errorLog(logsDirectory / "errorLog.txt", std::ios::app);
  errorLog << "Error Log: " << getCurrentTimeFormatted() << message << std::endl;
  errorLog.close();
}

void Logs::writeToInfoLog(const std::string &message) {
  ensureLogsDirectoryExists(logsDirectory);
  if (!checkFileExists(logsDirectory / "infoLog.txt")) {
    std::ofstream infoLog(logsDirectory / "infoLog.txt");
    infoLog << "Info Log: " << std::endl;
    infoLog.close();
  }
  std::cout << "Info Log: " << getCurrentTimeFormatted() << message << std::endl;
  std::ofstream infoLog(logsDirectory / "infoLog.txt", std::ios::app);
  infoLog << getCurrentTimeFormatted() << message << std::endl;
  infoLog.close();
}

bool Logs::checkFileExists(const fs::path &filePath) {
  std::ifstream file(filePath);
  const bool exists = file.good();
  file.close();
  return exists;
}

std::string Logs::getCurrentTimeFormatted() {
  const auto now = std::chrono::system_clock::now();
  const auto now_time = std::chrono::system_clock::to_time_t(now);
  const auto now_tm = *std::localtime(&now_time);

  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%d.%m.%y %H:%M:%S");
  return oss.str() + ": ";
}

void Logs::moveLogsDirectory(const fs::path &newLogsDirectory) {
  if (logsDirectory == newLogsDirectory) {
    return;
  }
  constexpr int maxRetries = 5;

  for (int attempt = 0; attempt < maxRetries; ++attempt) {
    try {
      if (!fs::exists(newLogsDirectory)) {
        fs::create_directories(newLogsDirectory);
      }

      if (fs::exists(logsDirectory / "errorLog.txt")) {
        fs::copy_file(logsDirectory / "errorLog.txt",
                      newLogsDirectory / "errorLog.txt",
                      fs::copy_options::overwrite_existing);
      }

      if (fs::exists(logsDirectory / "infoLog.txt")) {
        fs::copy_file(logsDirectory / "infoLog.txt",
                      newLogsDirectory / "infoLog.txt",
                      fs::copy_options::overwrite_existing);
      }

      if (checkFileExists(newLogsDirectory / "infoLog.txt")
          && checkFileExists(newLogsDirectory / "errorLog.txt")) {
        writeToInfoLog("Deleted old logs directory: " + logsDirectory.string());
        fs::remove_all(logsDirectory);
        logsDirectory = newLogsDirectory;
      } else {
        writeToErrorLog("Failed to move logs directory: " + newLogsDirectory.string());
        continue;
      }

      writeToInfoLog("Moved logs directory to: " + logsDirectory.string());
      return;
    } catch (const std::exception &e) {
      constexpr int retryDelayMs = 1000;
      writeToErrorLog(
        "Failed to move logs directory (attempt " + std::to_string(attempt + 1) + "): " + std::string(e.what()));
      std::this_thread::sleep_for(std::chrono::milliseconds(retryDelayMs));
    }
  }

  writeToErrorLog("Failed to move logs directory after " + std::to_string(maxRetries) + " attempts.");
  std::cerr << "Failed to move logs directory after " << maxRetries << " attempts." << std::endl;
}

fs::path Logs::getLogsDirectory() {
  return logsDirectory;
}
