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

/**
 * @class PasswordDialog
 * @brief A dialog for entering and verifying passwords.
 */
class PasswordDialog : public QDialog {
Q_OBJECT

public:
  /**
   * @brief Constructs the PasswordDialog object.
   * @param parent The parent widget.
   */
  PasswordDialog(QWidget *parent = nullptr);

  /**
   * @brief Destructor for the PasswordDialog class.
   */
  ~PasswordDialog() override = default;

  QVBoxLayout* layout; ///< Layout for the dialog.
  PasswordWidget* passwordField; ///< Widget for entering the password.
  QLabel* passwordLabel; ///< Label for the password field.
  QPushButton *okButton; ///< Button to confirm the password.

  std::unique_ptr<StyleSetter> styleSetter; ///< Utility for setting styles.
  std::unique_ptr<GetSettingsData> settingsData; ///< Utility for getting settings data.
  std::unique_ptr<CryptoDLL> cryptoDLL; ///< Utility for cryptographic operations.

private:
  int wrongInputCounter = 0; ///< Counter for wrong password attempts.

  /**
   * @brief Checks the entered password against the stored hash.
   * @return -1 if the password is correct or created, otherwise an error code.
   */
  int checkPassword();
};

#endif //FILEVAULTROOT_PASSWORDDIALOG_H