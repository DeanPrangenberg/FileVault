#ifndef NEWPASSWORDWIDGET_H
#define NEWPASSWORDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "MasterSettingsWidget.h"
#include "../smallElements/PasswordLineEdit.h"

class NewPasswordWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit NewPasswordWidget(QWidget *parent = nullptr);

  std::unique_ptr<PasswordWidget> oldPasswordInput;
  std::unique_ptr<PasswordWidget> newPasswordInput;
  std::shared_ptr<QPushButton> setPasswordButton;
};

#endif // NEWPASSWORDWIDGET_H