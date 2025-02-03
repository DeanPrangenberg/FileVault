//
// Created by prang on 29.01.2025.
//

#ifndef FILEVAULTROOT_PASSWORDDIALOG_H
#define FILEVAULTROOT_PASSWORDDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <memory>
#include "widgetClasses/elementWidgets/smallElements/PasswordLineEdit.h"
#include "styleClass/StyleSetter.h"
#include "../shared/src/settingsClasses/GetSettingsData.h"
#include "../DLLUtils/CryptoDLL.h"
#include "widgetClasses/Screens/SettingsScreenWidget.h"

class PasswordDialog : public QDialog {
Q_OBJECT

public:
  PasswordDialog(QWidget *parent = nullptr);
  QVBoxLayout* layout;
  PasswordWidget* passwordField;
  QLabel* passwordLabel;
  QPushButton *okButton;

  std::unique_ptr<StyleSetter> styleSetter;
  std::unique_ptr<GetSettingsData> settingsData;
  std::unique_ptr<CryptoDLL> cryptoDLL;
  std::vector<unsigned char> passwordHash;
  std::vector<unsigned char> savedPasswordHash;

private:
  bool firstStart = false;
  int wrongInputCounter = 0;
  int checkPassword();
};

#endif //FILEVAULTROOT_PASSWORDDIALOG_H
