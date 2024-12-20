#include "DecryptionLine.h"

DecryptionLine::DecryptionLine(QWidget *parent, const QString &path) : QWidget(parent) {
  DecryptionLineLayout = std::make_unique<QVBoxLayout>(this);

  PathLabel = std::make_unique<QLabel>("Path: " + path, this);
  SelectPath = std::make_unique<QCheckBox>(this);
  DecryptionStatus = std::make_shared<QLabel>("Decryption Status: ", this);


  DecryptionLineLayout->addWidget(PathLabel.get());
  DecryptionLineLayout->addWidget(SelectPath.get());
  DecryptionLineLayout->addWidget(DecryptionStatus.get());

  setLayout(DecryptionLineLayout.get());
}

