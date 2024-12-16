#include "NewPasswordWidget.h"

NewPasswordWidget::NewPasswordWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  title = std::make_unique<QLabel>("Set New Password", this);
  oldPasswordInput = std::make_unique<PasswordWidget>(this);
  oldPasswordInput->passwordField->setPlaceholderText("Old Password...");
  newPasswordInput = std::make_unique<PasswordWidget>(this);
  newPasswordInput->passwordField->setPlaceholderText("New Password...");
  setPasswordButton = std::make_shared<QPushButton>("Set Password", this);

  buttons.push_back(setPasswordButton);

  centralLayout->addWidget(title.get());
  centralLayout->addWidget(oldPasswordInput.get());
  centralLayout->addWidget(newPasswordInput.get());
  centralLayout->addWidget(setPasswordButton.get());

  applyStyle();
}