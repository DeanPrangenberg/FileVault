#include "EncryptionScreenWidget.h"

EncryptionScreenWidget::EncryptionScreenWidget(QWidget *parent) : QWidget(parent) {
  EncryptionScreenWidgetLayout = std::make_unique<QVBoxLayout>(this);

  EncryptionScreenTitle = std::make_unique<QLabel>("Encryption Screen", this);
  StartProcessButton = std::make_unique<QPushButton>("Start Process", this);
  FilePickerWidget = std::make_unique<FilePicker>(this, FilePickerType::ENCRYPT);

  configureUI();
}

void EncryptionScreenWidget::configureUI() {
  EncryptionScreenTitle->setAlignment(Qt::AlignCenter);
  EncryptionScreenWidgetLayout->addWidget(EncryptionScreenTitle.get());
  EncryptionScreenWidgetLayout->addWidget(FilePickerWidget.get());
  EncryptionScreenWidgetLayout->addWidget(StartProcessButton.get());

  StyleSettings.setButtonStyle(StartProcessButton.get());
}

void EncryptionScreenWidget::configureFilePicker() {

}