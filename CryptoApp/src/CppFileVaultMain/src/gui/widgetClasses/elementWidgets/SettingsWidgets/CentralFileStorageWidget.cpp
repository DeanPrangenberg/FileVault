#include "CentralFileStorageWidget.h"

CentralFileStorageWidget::CentralFileStorageWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("Central Storage");
  setMinimumHeight(140);
  forEncryptedFiles = std::make_shared<QCheckBox>("For Encrypted Files", this);
  forDecryptedFiles = std::make_shared<QCheckBox>("For Decrypted Files", this);
  selectStoragePathButton = std::make_shared<QPushButton>("Select Storage Path", this);
  storagePathLabel = std::make_unique<QLabel>("Current Path: ", this);

  auto checkBoxLayout = std::make_unique<QHBoxLayout>();
  checkBoxLayout->addWidget(forEncryptedFiles.get());
  checkBoxLayout->addWidget(forDecryptedFiles.get());

  centralLayout->addLayout(checkBoxLayout.release());
  centralLayout->addWidget(selectStoragePathButton.get());
  centralLayout->addWidget(storagePathLabel.get());

  checkBoxes.push_back(forEncryptedFiles);
  checkBoxes.push_back(forDecryptedFiles);
  buttons.push_back(selectStoragePathButton);

  applyStyle();
}