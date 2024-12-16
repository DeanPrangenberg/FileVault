#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include "../../../styleClass/StyleSetter.h"

class PasswordWidget : public QWidget {
Q_OBJECT

public:
  std::unique_ptr<QHBoxLayout> mainLayout;
  std::unique_ptr<QLineEdit> passwordField;
  std::unique_ptr<QPushButton> toggleButton;
  std::unique_ptr<QLabel> backgroundLabel;

  explicit PasswordWidget(QWidget *parent = nullptr) : QWidget(parent) {
    setMinimumSize(80, 25);
    backgroundLabel = std::make_unique<QLabel>(this);
    backgroundLabel->setMinimumSize(80, 25);
    backgroundLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout = std::make_unique<QHBoxLayout>(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    passwordField = std::make_unique<QLineEdit>(this);
    passwordField->setEchoMode(QLineEdit::Password);

    toggleButton = std::make_unique<QPushButton>(this);
    toggleButton->setIcon(QIcon(":/icons/Eye.png"));
    toggleButton->setFixedSize(15, 20);

    StyleSetter styleSetter;
    styleSetter.setPasswordToggleButtonStyle(toggleButton.get());
    styleSetter.setPasswordFieldStyle(passwordField.get());
    styleSetter.setLabelBackgroundStyle(backgroundLabel.get());

    mainLayout->addWidget(passwordField.get());
    mainLayout->addWidget(toggleButton.get());

    connect(toggleButton.get(), &QPushButton::clicked, this, &PasswordWidget::togglePasswordVisibility);
  }

  ~PasswordWidget() override = default;

  void togglePasswordVisibility() {
    if (passwordField->echoMode() == QLineEdit::Password) {
      passwordField->setEchoMode(QLineEdit::Normal);
      toggleButton->setIcon(QIcon(":/icons/NoEye.png"));
    } else {
      passwordField->setEchoMode(QLineEdit::Password);
      toggleButton->setIcon(QIcon(":/icons/Eye.png"));
    }
  }
};

#endif //PASSWORDLINEEDIT_H