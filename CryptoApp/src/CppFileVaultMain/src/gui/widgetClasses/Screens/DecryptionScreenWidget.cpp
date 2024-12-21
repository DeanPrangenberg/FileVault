#include "DecryptionScreenWidget.h"

DecryptionScreenWidget::DecryptionScreenWidget(QWidget *parent) : QWidget(parent) {
  DecryptionScreenWidgetLayout = std::make_unique<QVBoxLayout>(this);

  DecryptionScreenTitle = std::make_unique<QLabel>("Decryption Screen", this);
  StartProcessButton = std::make_unique<QPushButton>("Start Process", this);
  FilePickerWidget = std::make_unique<FilePicker>(this, FilePickerType::DECRYPT);

  configureUI();
}

void DecryptionScreenWidget::configureUI() {
  DecryptionScreenTitle->setAlignment(Qt::AlignCenter);
  DecryptionScreenWidgetLayout->addWidget(DecryptionScreenTitle.get());
  DecryptionScreenWidgetLayout->addWidget(FilePickerWidget.get());
  DecryptionScreenWidgetLayout->addWidget(StartProcessButton.get());

  StyleSettings.setButtonStyle(StartProcessButton.get());
  connect(StartProcessButton.get(), &QPushButton::clicked, this, &DecryptionScreenWidget::onStartProcessButtonClicked);
}

void DecryptionScreenWidget::onStartProcessButtonClicked() {
  std::vector<fs::path> filePathVector;
  std::vector<int> results;
  for (const auto &fileConfig: FilePickerWidget->getDecItems()) {
    fs::path filePath = fileConfig.Path.toStdWString();
    filePathVector.push_back(filePath);
  }
  HelperUtils helperUtils;
  results = helperUtils.decryptFiles(filePathVector);
}