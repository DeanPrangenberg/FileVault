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
  connect(StartProcessButton.get(), &QPushButton::clicked, this, &EncryptionScreenWidget::onStartProcessButtonClicked);
}

void EncryptionScreenWidget::onStartProcessButtonClicked() {
  std::vector<fs::path> filePathVector;
  std::vector<std::string> algorithmVector;
  std::vector<int> results;
  for (const auto &fileConfig: FilePickerWidget->getEncItems()) {
    fs::path filePath = fileConfig.Path.toStdWString();
    std::string algorithm = fileConfig.Algorithm.toStdString();
    filePathVector.push_back(filePath);
    algorithmVector.push_back(algorithm);
  }
  HelperUtils helperUtils;
  results = helperUtils.encryptFiles(filePathVector, algorithmVector);
}