#include "FileDeletionWidget.h"

FileDeletionWidget::FileDeletionWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  title = std::make_unique<QLabel>("File Deletion", this);
  deleteAfterEncryption = std::make_shared<QCheckBox>("Delete after Encryption", this);
  deleteAfterDecryption = std::make_shared<QCheckBox>("Delete after Decryption", this);

  checkBoxes.push_back(deleteAfterEncryption);
  checkBoxes.push_back(deleteAfterDecryption);

  centralLayout->addWidget(title.get());
  centralLayout->addWidget(deleteAfterEncryption.get());
  centralLayout->addWidget(deleteAfterDecryption.get());

  applyStyle();
}

