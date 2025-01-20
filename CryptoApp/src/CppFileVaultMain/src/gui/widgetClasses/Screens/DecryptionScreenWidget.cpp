#include "DecryptionScreenWidget.h"

/**
 * @brief Constructs the DecryptionScreenWidget object.
 * @param parent The parent widget.
 * @param statisticsPtr A shared pointer to the StatisticsScreenWidget.
 */
DecryptionScreenWidget::DecryptionScreenWidget(QWidget *parent, std::shared_ptr<StatisticsScreenWidget> statisticsPtr) : QWidget(parent) {
  statisticsScreenWidget = std::make_shared<StatisticsScreenWidget>();
  statisticsScreenWidget = statisticsPtr;

  DecryptionScreenWidgetLayout = std::make_unique<QVBoxLayout>(this);

  DecryptionScreenTitle = std::make_unique<QLabel>("Decryption Screen", this);
  StartProcessButton = std::make_unique<QPushButton>("Start Process", this);
  FilePickerWidget = std::make_unique<FilePicker>(this, FilePickerType::DECRYPT);

  configureUI();
}

/**
 * @brief Configures the user interface for the decryption screen.
 */
void DecryptionScreenWidget::configureUI() {
  DecryptionScreenTitle->setAlignment(Qt::AlignCenter);
  DecryptionScreenWidgetLayout->addWidget(DecryptionScreenTitle.get());
  DecryptionScreenWidgetLayout->addWidget(FilePickerWidget.get());
  DecryptionScreenWidgetLayout->addWidget(StartProcessButton.get());

  StyleSettings.setButtonStyle(StartProcessButton.get());
  connect(StartProcessButton.get(), &QPushButton::clicked, this, &DecryptionScreenWidget::onStartProcessButtonClicked);
}

/**
 * @brief Slot for handling the start process button click.
 */
void DecryptionScreenWidget::onStartProcessButtonClicked() {
  std::vector<fs::path> filePathVector;
  std::unordered_map<std::string, int> results;
  for (const auto &fileConfig: FilePickerWidget->getDecItems()) {
    fs::path filePath = fileConfig.Path.toStdWString();
    filePathVector.push_back(filePath);
  }
  HelperUtils helperUtils;
  results = helperUtils.decryptFiles(filePathVector);

  std::vector<int> resultsVec;

  for (const auto result: results) {
    resultsVec.push_back(result.second);
  }

  FilePickerWidget->removeDecItems(resultsVec);

  for (const auto &result: results) {
    if (result.second == -1) {
      statisticsScreenWidget->updateDecryptedFilesCount(1);
      if (result.first == "AES128") {
        statisticsScreenWidget->updateAes128Count(-1);
      } else if (result.first == "AES256") {
        statisticsScreenWidget->updateAes256Count(-1);
      }
    } else {
      qDebug() << "Decryption failed code:" << result;
    }
  }
}