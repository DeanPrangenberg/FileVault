//
// Created by prang on 29.01.2025.
//

#include "PasswordDialog.h"

/**
 * @brief Constructs the PasswordDialog object.
 * @param parent The parent widget.
 */
PasswordDialog::PasswordDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Enter Password");
  setFixedSize(400, 140);
  setModal(true);
  styleSetter = std::make_unique<StyleSetter>();
  settingsData = std::make_unique<GetSettingsData>();
  cryptoDLL = std::make_unique<CryptoDLL>();

  layout = new QVBoxLayout(this);
  passwordField = new PasswordWidget(this);
  passwordLabel = new QLabel("Enter Password", this);
  okButton = new QPushButton("OK", this);

  layout->addWidget(passwordLabel);
  layout->addWidget(passwordField);
  layout->addWidget(okButton);

  styleSetter->setLabelStyleNoBorder(passwordLabel);
  styleSetter->setButtonStyle(okButton);

  QString objectName = "QDialog";
  setObjectName(objectName);
  QString gradientStyle = QString("QDialog#%1 {"
                                  "background: qradialgradient("
                                  "cx:0.1, cy:0.2, "
                                  "radius:1.0, "
                                  "fx:0.1, fy:0.2, "
                                  "stop:0 rgba(100, 43, 115, 1), "
                                  "stop:0.9 rgba(25, 0, 51, 1))}").arg(this->objectName()); // Dark mix of blue and purple
  setStyleSheet(gradientStyle);

  setLayout(layout);

  auto savedPasswordHash = settingsData->GetPasswordHash();
  auto savedSalt = settingsData->GetSalt();

  if (savedPasswordHash.empty() || savedSalt.empty()) {
    passwordLabel->setText("Create new Password");
  }

  connect(okButton, &QPushButton::clicked, [this] {
    if (checkPassword() == -1) {
      accept();
      std::cout << "Password correct closing Dialog" << std::endl;
    } else {
      wrongInputCounter++;
      if (wrongInputCounter < 3) {
        QMessageBox::warning(this, "Error", "Wrong Password entered (" + QString::number(wrongInputCounter) + "/3)");
      } else {
        QMessageBox::warning(this, "Error", "Too many wrong inputs");
        QApplication::quit();
      }
    }
  });
}

/**
 * @brief Checks the entered password against the stored hash.
 * @return -1 if the password is correct or created, otherwise an error code.
 */
int PasswordDialog::checkPassword() {
  if (passwordField->passwordField->text().isEmpty()) {
    return 1; // Empty password
  }

  try {
    auto savedPasswordHash = settingsData->GetPasswordHash();
    auto savedSalt = settingsData->GetSalt();

    // Create new password if none exists
    if (savedPasswordHash.empty() || savedSalt.empty()) {
      const auto passwordStr = passwordField->passwordField->text().toStdString();
      std::vector<unsigned char> password(passwordStr.begin(), passwordStr.end());

      // Generate salt and hash
      std::vector<unsigned char> salt(16);
      std::vector<unsigned char> newPasswordHash(64);

      cryptoDLL->generateRandomBytes(salt.size(), salt.data());
      cryptoDLL->GeneratePasswordHash(
          password.data(),          // non-const data
          salt.data(),              // non-const data
          newPasswordHash.data(),
          password.size(),
          salt.size(),
          newPasswordHash.size()
      );

      // Store new credentials
      settingsData->SetPasswordHash(newPasswordHash);
      settingsData->SetSalt(salt);

      std::cout << "New password set successfully.\n";
      return -1; // Success: password created
    }

    // Verify existing password
    auto passwordStr = passwordField->passwordField->text().toStdString();
    std::vector<unsigned char> password(passwordStr.begin(), passwordStr.end());

    std::vector<unsigned char> inputHash(64);
    cryptoDLL->GeneratePasswordHash(
        password.data(),    // Safe const cast
        savedSalt.data(),   // Safe const cast
        inputHash.data(),
        password.size(),
        savedSalt.size(),
        inputHash.size()
    );

    // Debug output
    std::cout << "Stored hash: ";
    for (const auto& byte : savedPasswordHash) {
      std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::cout << "\nComputed hash: ";
    for (const auto& byte : inputHash) {
      std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::cout << std::endl;

    // Hash comparison
    if (inputHash.size() != savedPasswordHash.size()) {
      std::cerr << "Hash size mismatch!\n";
      return 2;
    }

    if (inputHash != savedPasswordHash) {
      std::cerr << "Incorrect password!\n";
      return 3;
    }

    std::cout << "Password correct.\n";
    return -1; // Success: valid password

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 4; // System error
  }
}