#include "EncryptionLine.h"

EncryptionLine::EncryptionLine(QWidget *parent, const QString &path) : QWidget(parent) {
  EncryptionLineLayout = std::make_unique<QVBoxLayout>(this);

  PathLabel = std::make_unique<QLabel>("Path: " + path, this);
  SelectPath = std::make_unique<QCheckBox>(this);
  EncryptionStatus = std::make_shared<QLabel>("Decryption Status: ", this);


  EncryptionLineLayout->addWidget(PathLabel.get());
  EncryptionLineLayout->addWidget(SelectPath.get());
  EncryptionLineLayout->addWidget(EncryptionStatus.get());

  setLayout(EncryptionLineLayout.get());
}

