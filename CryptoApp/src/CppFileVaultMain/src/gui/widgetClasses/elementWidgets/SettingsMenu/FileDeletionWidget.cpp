#include "FileDeletionWidget.h"

FileDeletionWidget::FileDeletionWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  layout = new QVBoxLayout(this);
  title = new QLabel("File Deletion", this);
  deleteAfterEncryption = new QCheckBox("Delete after Encryption", this);
  deleteAfterDecryption = new QCheckBox("Delete after Decryption", this);

  layout->addWidget(title);
  layout->addWidget(deleteAfterEncryption);
  layout->addWidget(deleteAfterDecryption);

  setLayout(layout);
}