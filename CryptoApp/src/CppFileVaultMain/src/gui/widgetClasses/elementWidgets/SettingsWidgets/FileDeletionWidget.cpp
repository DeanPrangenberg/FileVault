#include "FileDeletionWidget.h"

FileDeletionWidget::FileDeletionWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("File Deletion");
  setMinimumHeight(130);
  deleteAfterEncryption = std::make_shared<QCheckBox>("Delete after Encryption", this);
  deleteAfterDecryption = std::make_shared<QCheckBox>("Delete after Decryption", this);

  checkBoxes.push_back(deleteAfterEncryption);
  checkBoxes.push_back(deleteAfterDecryption);


  centralLayout->addWidget(deleteAfterEncryption.get());
  centralLayout->addWidget(deleteAfterDecryption.get());

  applyStyle();
}

