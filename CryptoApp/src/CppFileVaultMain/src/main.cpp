#include <QApplication>
#include <chrono>
#include <thread>
#include "../../shared/src/settingsClasses/Logs.h"
#include "gui/FileVaultGui.h"
#include "BackendTester/BackendTester.h"

constexpr bool backendtest = false;

#define dockerHash "812eaa5336593d48d4ab79314441b7c5310935f850e5d79b32e299032ce29e94"

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

int main(int argc, char *argv[]) {
  if (backendtest) {
    startDBContainer();

    BackendTester backendTester(50);

    stopDBContainer();
    return 0;
  }

  QApplication app(argc, argv);
  startDBContainer();
  system(".\\RustFileCopy.exe");

  QObject::connect(&app, &QApplication::aboutToQuit, []() {
    stopDBContainer();
  });

  FileVaultGui fileVaultGui;
  fileVaultGui.setWindowIcon(QIcon(":/icons/Icon.png"));
  fileVaultGui.show();

  return app.exec();
}