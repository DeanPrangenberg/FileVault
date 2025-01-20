#include "EncryptionScreenWidget.h"

/**
 * @brief Constructs the EncryptionScreenWidget object.
 * @param parent The parent widget.
 * @param statisticsPtr A shared pointer to the StatisticsScreenWidget.
 */
EncryptionScreenWidget::EncryptionScreenWidget(QWidget *parent, std::shared_ptr<StatisticsScreenWidget> statisticsPtr) : QWidget(parent) {
  EncryptionScreenWidgetLayout = std::make_unique<QVBoxLayout>(this);

  statisticsScreenWidget = std::make_shared<StatisticsScreenWidget>();
  statisticsScreenWidget = statisticsPtr;

  EncryptionScreenTitle = std::make_unique<QLabel>("Encryption Screen", this);
  StartProcessButton = std::make_unique<QPushButton>("Start Process", this);
  FilePickerWidget = std::make_unique<FilePicker>(this, FilePickerType::ENCRYPT);

  configureUI();
}

/**
 * @brief Configures the user interface for the encryption screen.
 */
void EncryptionScreenWidget::configureUI() {
  EncryptionScreenTitle->setAlignment(Qt::AlignCenter);
  EncryptionScreenWidgetLayout->addWidget(EncryptionScreenTitle.get());
  EncryptionScreenWidgetLayout->addWidget(FilePickerWidget.get());
  EncryptionScreenWidgetLayout->addWidget(StartProcessButton.get());

  StyleSettings.setButtonStyle(StartProcessButton.get());
  connect(StartProcessButton.get(), &QPushButton::clicked, this, &EncryptionScreenWidget::onStartProcessButtonClicked);
}

/**
 * @brief Slot for handling the start process button click.
 */
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

  FilePickerWidget->removeEncItems(results);

  int index = 0;
  for (const auto &result: results) {
    if (result == -1) {
      statisticsScreenWidget->updateEncryptedFilesCount(1);
      if (algorithmVector[index] == "AES-128") {
        statisticsScreenWidget->updateAes128Count(1);
      } else {
        statisticsScreenWidget->updateAes256Count(1);
      }
    } else {
      qDebug() << "Encryption failed code:" << result;
    }
    ++index;
  }
}