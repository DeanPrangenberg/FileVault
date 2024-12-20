#include "NewPasswordWidget.h"

NewPasswordWidget::NewPasswordWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("Set New Password");
  setMinimumHeight(150);
  oldPasswordInput = std::make_unique<PasswordWidget>(this);
  oldPasswordInput->passwordField->setPlaceholderText("Old Password...");
  newPasswordInput = std::make_unique<PasswordWidget>(this);
  newPasswordInput->passwordField->setPlaceholderText("New Password...");
  setPasswordButton = std::make_shared<QPushButton>("Set Password", this);

  buttons.push_back(setPasswordButton);


  centralLayout->addWidget(oldPasswordInput.get());
  centralLayout->addWidget(newPasswordInput.get());
  centralLayout->addWidget(setPasswordButton.get());

  applyStyle();
}