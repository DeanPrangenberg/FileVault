#include <QApplication>
#include <chrono>
#include <thread>
#include "../../shared/src/settingsClasses/Logs.h"
#include "gui/FileVaultGui.h"
#include "BackendTester/BackendTester.h"

constexpr bool backendtest = false;
constexpr bool release = true;

std::string dockerHash;

/**
 * @brief Starts the Docker container for the database.
 */
void startDBContainer() {
  std::string command = "docker start " + std::string(dockerHash);
  system(command.c_str());
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

/**
 * @brief Stops the Docker container for the database.
 */
void stopDBContainer() {
  std::string command = "docker stop " + std::string(dockerHash);
  system(command.c_str());
}


std::string readFileToString(const std::string &filePath) {
  std::ifstream fileStream(filePath);
  if (!fileStream.is_open()) {
    throw std::runtime_error("Konnte die Datei nicht öffnen: " + filePath);
  }

  std::stringstream buffer;
  buffer << fileStream.rdbuf();
  return buffer.str();
}

int main(int argc, char *argv[]) {
  dockerHash = readFileToString("dockerContainerHash.txt");
  if (backendtest) {
    startDBContainer();

    BackendTester backendTester(50);

    stopDBContainer();
    return 0;
  }

  QApplication app(argc, argv);
  startDBContainer();
  if(!release) system(".\\RustFileCopy.exe");

  QObject::connect(&app, &QApplication::aboutToQuit, []() {
    stopDBContainer();
  });

  FileVaultGui fileVaultGui;
  fileVaultGui.setWindowTitle("FileVault");
  fileVaultGui.setStyleSheet("QWidget {color: #FFFFFF; }");
  fileVaultGui.setWindowIcon(QIcon(":/icons/Icon.png"));
  fileVaultGui.show();

  return app.exec();
}