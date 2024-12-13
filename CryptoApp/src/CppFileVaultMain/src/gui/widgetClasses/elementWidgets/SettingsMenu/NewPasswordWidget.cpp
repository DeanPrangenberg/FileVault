#include "NewPasswordWidget.h"

NewPasswordWidget::NewPasswordWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  NewPasswordWidgetLayout = new QVBoxLayout(this);
  title = new QLabel("Set New Password", this);
  oldPasswordInput = new QLineEdit(this);
  oldPasswordInput->setPlaceholderText("Old Password");
  newPasswordInput = new QLineEdit(this);
  newPasswordInput->setPlaceholderText("New Password");
  setPasswordButton = new QPushButton("Set Password", this);

  NewPasswordWidgetLayout->addWidget(title);
  NewPasswordWidgetLayout->addWidget(oldPasswordInput);
  NewPasswordWidgetLayout->addWidget(newPasswordInput);
  NewPasswordWidgetLayout->addWidget(setPasswordButton);

  setLayout(NewPasswordWidgetLayout);
}

NewPasswordWidget::~NewPasswordWidget() {
  delete NewPasswordWidgetLayout;
}