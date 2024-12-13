#ifndef FILEVAULTGUI_H
#define FILEVAULTGUI_H

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include "styleClass/StyleSetter.h"

class FileVaultGui : public QMainWindow {
Q_OBJECT

public:
  explicit FileVaultGui(QMainWindow *parent = nullptr);
  ~FileVaultGui() override;

private:
  // UI Elements
  QVBoxLayout *mainLayout;
  QHBoxLayout *SplitterLayout;
  QStackedWidget *screenStack;

  QWidget *sidebar;
  QVBoxLayout *sideBarLayout;
  QLabel *Logo;
  QPushButton *StatisticsSwitch;
  QPushButton *EncryptSwitch;
  QPushButton *DecryptSwitch;
  QPushButton *SettingsSwitch;

  // Variables
  int screenWidth = 1000;
  int screenHeight = 600;
  QHash<QString, QWidget *> screenHash;

  // Functions
  void setupUi();

  void setupSideBar();

  void setupStackedWidget();
};

#endif // FILEVAULTGUI_H