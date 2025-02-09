#include <QMessageBox>
#include "NewPasswordWidget.h"
#include "src/settingsClasses/GetSettingsData.h"
#include "../DLLUtils/CryptoDLL.h"

/**
 * @brief Constructs a NewPasswordWidget.
 *
 * @param parent The parent widget.
 */
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

  connect(setPasswordButton.get(), &QPushButton::clicked, this, &NewPasswordWidget::setNewPassword);

  applyStyle();
}

/**
 * @brief Sets a new password.
 *
 * This function checks if the old and new password fields are filled.
 * It verifies the old password against the saved hash and salt.
 * If the old password is correct, it hashes the new password and updates the settings.
 * Displays appropriate messages for success or failure.
 */
void NewPasswordWidget::setNewPassword() {
  if (oldPasswordInput->passwordField->text().isEmpty() || newPasswordInput->passwordField->text().isEmpty()) {
    QMessageBox::warning(this, "Error", "Please fill out all fields.");
    return;
  }

  GetSettingsData settingsData;
  CryptoDLL cryptoDLL;
  auto savedPasswordHash = settingsData.GetPasswordHash();
  auto savedSalt = settingsData.GetSalt();

  // Check for valid saved data
  if (savedSalt.empty() || savedPasswordHash.empty()) {
    QMessageBox::warning(this, "Error", "No saved password found.");
    return;
  }

  // Convert password to UTF-8 bytes
  QByteArray oldPasswordBytes = oldPasswordInput->passwordField->text().toUtf8();
  std::vector<unsigned char> oldPasswordInputVec(oldPasswordBytes.begin(), oldPasswordBytes.end());
  if (oldPasswordInputVec.empty()) {
    QMessageBox::warning(this, "Error", "Old password is empty.");
    return;
  }

  // Allocate hash buffer on the heap (sicherer)
  const size_t hashSize = 64;
  auto oldEnteredPasswordHash = std::make_unique<unsigned char[]>(hashSize);

  // Fixed sizes for salt (z. B. 16 Bytes)
  const size_t saltSize = 16;

  // Call DLL with correct parameters
  cryptoDLL.GeneratePasswordHash(
      oldPasswordInputVec.data(),
      savedSalt.data(),
      oldEnteredPasswordHash.get(),
      oldPasswordInputVec.size(),
      saltSize,
      hashSize
  );

  // Compare hashes safely
  std::vector<unsigned char> enteredHash(oldEnteredPasswordHash.get(), oldEnteredPasswordHash.get() + hashSize);
  if (savedPasswordHash.size() == enteredHash.size() &&
      memcmp(savedPasswordHash.data(), enteredHash.data(), savedPasswordHash.size()) == 0) {

      auto newPasswordInputStr = newPasswordInput->passwordField->text().toUtf8();
      std::vector<unsigned char> newPasswordInputVec(newPasswordInputStr.begin(), newPasswordInputStr.end());
      std::vector<unsigned char> newPasswordHash(hashSize);

      cryptoDLL.GeneratePasswordHash(
          newPasswordInputVec.data(),
          savedSalt.data(),
          newPasswordHash.data(),
          newPasswordInputVec.size(),
          saltSize,
          hashSize
      );
      settingsData.SetPasswordHash(newPasswordHash);

      QMessageBox::information(this, "Success", "Password changed successfully.");

  } else {
    QMessageBox::warning(this, "Error", "Old password is incorrect.");
  }
}