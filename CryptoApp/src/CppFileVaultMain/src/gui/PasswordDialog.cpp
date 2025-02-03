//
// Created by prang on 29.01.2025.
//

#include "PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Enter Password");
  setFixedSize(400, 200);
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

  setLayout(layout);

  connect(okButton, &QPushButton::clicked, [this] {
    if (checkPassword() == -1) {
      wrongInputCounter = 0;
      accept();
      close();
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

int PasswordDialog::checkPassword() {
  if (passwordField->passwordField->text().isEmpty()) {
    return 1;
  } else {
    try {
      savedPasswordHash = settingsData->GetPasswordHash();

      if (savedPasswordHash.empty()) {
        firstStart = true;
        std::vector<unsigned char> password = {'1', '2', '3', '4'};
        std::vector<unsigned char> salt(16);
        std::vector<unsigned char> passwordHashGen(64);
        cryptoDLL->generateRandomBytes(salt.size(), salt.data());
        passwordHash.resize(64);

        cryptoDLL->GeneratePasswordHash(
            password.data(),
            salt.data(),
            passwordHash.data(),
            password.size(),
            salt.size(),
            passwordHash.size()
        );

        if (passwordHash.size() != passwordHashGen.size()) {
          std::cerr << "Password hash size does not match" << std::endl;
          return 2;
        }
      }

      auto passwordStr = passwordField->passwordField->text().toStdString();
      auto passwordVec = std::vector<unsigned char>(passwordStr.begin(), passwordStr.end());

      std::cout << "Password: " << passwordStr << std::endl;

      passwordHash.resize(64);
      cryptoDLL->GeneratePasswordHash(
          passwordVec.data(),
          SettingsScreenWidget::salt.data(),
          passwordHash.data(),
          passwordVec.size(),
          SettingsScreenWidget::salt.size(),
          passwordHash.size()
      );

      std::cout << "Password hash: ";
      for (const auto &byte : passwordHash) {
        std::cout << std::setw(2) << std::hex << static_cast<int>(byte);
      }
      std::cout << std::endl;
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }

  std::cout << "Comparing password hashes" << std::endl;

  if (passwordHash.size() != savedPasswordHash.size()) {
    std::cerr << "Password hash size does not match" << std::endl;
    return 2;
  }

  if (passwordHash != savedPasswordHash) {
    std::cerr << "Password hash does not match" << std::endl;
    return 3;
  }

  std::cout << "Password hashes match" << std::endl;

  std::cout << "Password hash: " << std::endl;
  for (const auto &byte : passwordHash) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
  }
  std::cout << std::endl;

  std::cout << "Saved password hash: " << std::endl;
  for (const auto &byte : savedPasswordHash) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
  }
  std::cout << std::endl;

  return -1;
}
