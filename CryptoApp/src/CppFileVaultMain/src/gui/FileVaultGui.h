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

class FileVaultGui : public QMainWindow {
Q_OBJECT

public:
  explicit FileVaultGui(QMainWindow *parent = nullptr);
  ~FileVaultGui() override;

private:
  // UI Elements
  std::unique_ptr<QWidget> centralWidget;
  std::unique_ptr<QVBoxLayout> mainLayout;
  std::unique_ptr<QHBoxLayout> SplitterLayout;
  std::unique_ptr<QStackedWidget> screenStack;

  std::unique_ptr<QWidget> sidebar;
  std::unique_ptr<QVBoxLayout> sideBarLayout;
  std::unique_ptr<QLabel> Logo;
  std::unique_ptr<QPushButton> StatisticsSwitch;
  std::unique_ptr<QPushButton> EncryptSwitch;
  std::unique_ptr<QPushButton> DecryptSwitch;
  std::unique_ptr<QPushButton> SettingsSwitch;

  // Variables
  int screenWidth = 1000;
  int screenHeight = 600;
  QHash<QString, std::shared_ptr<QWidget>> screenHash;

  // Functions
  void setupUi();
  void setupSideBar();
  void setupStackedWidget();
};

#endif // FILEVAULTGUI_H