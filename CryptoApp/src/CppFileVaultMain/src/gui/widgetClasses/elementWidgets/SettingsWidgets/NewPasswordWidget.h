#ifndef NEWPASSWORDWIDGET_H
#define NEWPASSWORDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "MasterSettingsWidget.h"
#include "../smallElements/PasswordLineEdit.h"

/**
 * @class NewPasswordWidget
 * @brief A widget for setting a new password.
 *
 * This widget allows the user to input their old password and set a new password.
 */
class NewPasswordWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs a NewPasswordWidget.
   *
   * @param parent The parent widget.
   */
  explicit NewPasswordWidget(QWidget *parent = nullptr);

  /// Input field for the old password.
  std::unique_ptr<PasswordWidget> oldPasswordInput;

  /// Input field for the new password.
  std::unique_ptr<PasswordWidget> newPasswordInput;

  /// Button to set the new password.
  std::shared_ptr<QPushButton> setPasswordButton;

private:
  /**
   * @brief Sets a new password.
   *
   * This function checks if the old and new password fields are filled.
   * It verifies the old password against the saved hash and salt.
   * If the old password is correct, it hashes the new password and updates the settings.
   * Displays appropriate messages for success or failure.
   */
  void setNewPassword();
};

#endif // NEWPASSWORDWIDGET_H