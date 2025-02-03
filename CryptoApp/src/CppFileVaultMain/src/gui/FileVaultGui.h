#ifndef FILEVAULTGUI_H
#define FILEVAULTGUI_H

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <memory>
#include <QHash>
#include "styleClass/StyleSetter.h"
#include "PasswordDialog.h"

/**
 * @class FileVaultGui
 * @brief Main GUI class for the FileVault application.
 */
class FileVaultGui : public QMainWindow {
Q_OBJECT

public:
  /**
   * @brief Constructs the FileVaultGui object.
   * @param parent The parent QMainWindow.
   */
  explicit FileVaultGui(QMainWindow *parent = nullptr);

  /**
   * @brief Destructor for the FileVaultGui class.
   */
  ~FileVaultGui() override;

private:
  // UI Elements
  std::unique_ptr<QWidget> centralWidget; ///< Central widget of the main window.
  std::unique_ptr<QHBoxLayout> SplitterLayout; ///< Layout for the main window.
  std::unique_ptr<QStackedWidget> screenStack; ///< Stacked widget for different screens.

  std::unique_ptr<QWidget> sidebar; ///< Sidebar widget.
  std::unique_ptr<QVBoxLayout> sideBarLayout; ///< Layout for the sidebar.
  std::unique_ptr<QLabel> Logo; ///< Logo label.
  std::unique_ptr<QPushButton> StatisticsSwitch; ///< Button to switch to the Statistics screen.
  std::unique_ptr<QPushButton> EncryptSwitch; ///< Button to switch to the Encrypt screen.
  std::unique_ptr<QPushButton> DecryptSwitch; ///< Button to switch to the Decrypt screen.
  std::unique_ptr<QPushButton> SettingsSwitch; ///< Button to switch to the Settings screen.

  // Variables
  int screenWidth = 1000; ///< Width of the main window.
  int screenHeight = 600; ///< Height of the main window.
  QHash<QString, std::shared_ptr<QWidget>> screenHash; ///< Hash map to store screen widgets.

  // Functions
  /**
   * @brief Sets up the user interface.
   */
  void setupUi();

  /**
   * @brief Sets up the sidebar with buttons and logo.
   */
  void setupSideBar();

  /**
   * @brief Sets up the stacked widget with different screens.
   */
  void setupStackedWidget();

  /**
   * @brief Switches the current screen in the stacked widget.
   * @param screenName The name of the screen to switch to.
   */
  void switchScreens(const QString &screenName);
};

#endif // FILEVAULTGUI_H