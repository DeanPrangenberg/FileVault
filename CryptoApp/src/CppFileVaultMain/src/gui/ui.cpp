//
// Created by prang on 29.09.2024.
//

#include <MainWindow.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

void RansomWindow::loadMainWidget() {
  auto *mainWidget = new QWidget(this);
  mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  mainWidget->setStyleSheet(
      "QWidget {"
      "   background-color: #2E2E2E;"
      "}"
      "QLabel {"
      "   color: #FFFFFF;"
      "}"
      "QTextEdit {"
      "   background-color: #3C3F41;"
      "   color: #FFFFFF;"
      "   border: 1px solid #FFFFFF;"
      "   border-radius: 5px;"
      "   padding: 5px;"
      "}"
      "QPushButton {"
      "   background-color: #4C4F51;"
      "   color: #FFFFFF;"
      "   border: 1px solid #FFFFFF;"
      "   border-radius: 5px;"
      "   padding: 5px;"
      "}"
      "QPushButton:hover {"
      "   background-color: #5C5F61;"
      "}"
      "QHBoxLayout, QVBoxLayout {"
      "   spacing: 10px;"
      "}"
  );

  auto *mainLayout = new QVBoxLayout(mainWidget);

  auto *statusLabel = new QLabel("Status: Press the Pay button to pay the Ransom"); // Beispieltext
  statusLabel->setMaximumHeight(50); // Mindesthöhe setzen
  statusLabel->setStyleSheet(
      "QLabel {"
      "   border: 1px solid #FFFFFF;"
      "   border-radius: 5px;"
      "   background-color: #3C3F41;"
      "   padding: 5px;"
      "}"
  );
  mainLayout->addWidget(statusLabel);

  auto *keyTextLabel = new QLabel("KEY: ");
  mainLayout->addWidget(keyTextLabel);

  keyInputLineEdit = new QTextEdit();
  keyInputLineEdit->setPlaceholderText("Enter KEY....");
  mainLayout->addWidget(keyInputLineEdit);

  auto *ivTextLabel = new QLabel("IV: ");
  mainLayout->addWidget(ivTextLabel);

  ivInputLineEdit = new QTextEdit();
  ivInputLineEdit->setPlaceholderText("Enter IV....");
  mainLayout->addWidget(ivInputLineEdit);

  auto *buttonLayout = new QHBoxLayout();

  auto *payRansomButton = new QPushButton("Press me to pay");
  buttonLayout->addWidget(payRansomButton);

  auto *decryptionButton = new QPushButton("Press me for Decryption");
  buttonLayout->addWidget(decryptionButton);

  mainLayout->addLayout(buttonLayout);

  auto *mainWindowLayout = new QVBoxLayout(this);
  mainWindowLayout->setContentsMargins(0, 0, 0, 0);
  mainWindowLayout->addWidget(mainWidget);
  setLayout(mainWindowLayout);

  connect(payRansomButton, &QPushButton::clicked, this, &RansomWindow::payRansom);
  connect(decryptionButton, &QPushButton::clicked, this, &RansomWindow::startDecryption);
}