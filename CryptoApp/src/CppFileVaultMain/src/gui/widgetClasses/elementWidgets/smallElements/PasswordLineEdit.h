#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include "../../../styleClass/StyleSetter.h"

/**
 * @brief The PasswordWidget class provides a widget for entering and toggling the visibility of a password.
 */
class PasswordWidget : public QWidget {
Q_OBJECT

public:
  std::unique_ptr<QHBoxLayout> mainLayout; ///< Layout for the password widget.
  std::unique_ptr<QLineEdit> passwordField; ///< Field for entering the password.
  std::unique_ptr<QPushButton> toggleButton; ///< Button to toggle password visibility.
  std::unique_ptr<QLabel> backgroundLabel; ///< Background label for the password widget.

  /**
   * @brief Constructs the PasswordWidget object.
   * @param parent The parent widget.
   */
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
    passwordField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toggleButton = std::make_unique<QPushButton>(this);
    toggleButton->setIcon(QIcon(":/icons/Eye.png"));
    toggleButton->setFixedSize(15, 20);

    StyleSetter styleSetter;
    styleSetter.setPasswordToggleButtonStyle(toggleButton.get());
    styleSetter.setPasswordFieldStyle(passwordField.get());
    styleSetter.setLabelBackgroundStyle(backgroundLabel.get());

    mainLayout->addWidget(passwordField.get());
    mainLayout->addWidget(toggleButton.get());

    setLayout(mainLayout.get());

    connect(toggleButton.get(), &QPushButton::clicked, this, &PasswordWidget::togglePasswordVisibility);
  }

  /**
   * @brief Destructor for the PasswordWidget object.
   */
  ~PasswordWidget() override = default;

  /**
   * @brief Toggles the visibility of the password in the password field.
   */
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