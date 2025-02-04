#include "StatisticsScreenWidget.h"

/**
 * @brief Constructs the StatisticsScreenWidget object.
 * @param parent The parent widget.
 */
StatisticsScreenWidget::StatisticsScreenWidget(QWidget *parent) : QWidget(parent),
                                                                  aes128Count(0), aes256Count(0),
                                                                  lostFilesCount(0), FoundFilesCount(0),
                                                                  decryptedFilesCount(0), encryptedFilesCount(0),
                                                                  databaseSize(0), lifetimeAes128Count(0),
                                                                  lifetimeAes256Count(0) {
  infoTextsLayout = std::make_unique<QGridLayout>();
  titleLabel = std::make_unique<QLabel>(this);

  checkFileStateButton = std::make_unique<QPushButton>("Check File State", this);
  tryRepairFileButton = std::make_unique<QPushButton>("Try File Repair", this);

  algoCakeDiagram = std::make_unique<CakeDiagram>(this);
  LostFilesCakeDiagram = std::make_unique<CakeDiagram>(this);

  decryptedFilesLabel = std::make_unique<QLabel>(this);
  encryptedFilesLabel = std::make_unique<QLabel>(this);
  sizeOfDatabaseLabel = std::make_unique<QLabel>(this);

  algoStatsWidget = std::make_unique<AlgorithmStatisticWidget>(this, algoFileCounts);

  infoTextsLayout->addWidget(titleLabel.get(), 0, 0, 1, 2);
  infoTextsLayout->addWidget(algoCakeDiagram.get(), 1, 0, 1, 1);
  infoTextsLayout->addWidget(LostFilesCakeDiagram.get(), 1, 1, 1, 1);
  infoTextsLayout->addWidget(decryptedFilesLabel.get(), 2, 0, 1, 1);
  infoTextsLayout->addWidget(algoStatsWidget.get(), 2, 1, 1, 1);
  infoTextsLayout->addWidget(encryptedFilesLabel.get(), 3, 0, 1, 1);
  infoTextsLayout->addWidget(sizeOfDatabaseLabel.get(), 3, 1, 1, 1);
  infoTextsLayout->addWidget(checkFileStateButton.get(), 4, 0, 1, 1);
  infoTextsLayout->addWidget(tryRepairFileButton.get(), 4, 1, 1, 1);

  helperUtils = std::make_unique<HelperUtils>();

  setLayout(infoTextsLayout.get());
  SetupUI();

  ensureStatisticsFileExists("statistics.json");
  loadStatisticsFromJson("statistics.json");
  updateStatistics();
}

/**
 * @brief Sets up the user interface for the widget.
 */
void StatisticsScreenWidget::SetupUI() {
  titleLabel->setText("Statistics");
  titleLabel->setMaximumHeight(20);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet("background: transparent;");

  checkFileStateButton->setMinimumHeight(40);
  tryRepairFileButton->setMinimumHeight(40);

  StyleSetter styleSetter;
  styleSetter.setButtonStyle(checkFileStateButton.get());
  styleSetter.setButtonStyle(tryRepairFileButton.get());
  styleSetter.setLabelBackgroundStyle(decryptedFilesLabel.get());
  styleSetter.setLabelBackgroundStyle(encryptedFilesLabel.get());
  styleSetter.setLabelBackgroundStyle(sizeOfDatabaseLabel.get());

  connect(checkFileStateButton.get(), &QPushButton::clicked, this, [this](){
    FoundFilesCount = 0;
    lostFilesCount = 0;
    auto results = helperUtils->checkDBFileState();
    updateFoundFilesCount(results[0]);
    updateLostFilesCount(results[1]);
  });

  connect(tryRepairFileButton.get(), &QPushButton::clicked, this, [this](){
    helperUtils->repairAllLostStruct();
  });

  updateStatistics();
}

/**
 * @brief Updates the statistics displayed in the widget.
 */
void StatisticsScreenWidget::updateStatistics() {
  RestApiDLL restApiDll;

  //databaseSize = restApiDll.getDatabaseFileSize();
  databaseSize = 0;

  algoCakeDiagram->reset();
  LostFilesCakeDiagram->reset();

  decryptedFilesLabel->setText(QString("Total Decrypted Files: %1").arg(decryptedFilesCount));
  encryptedFilesLabel->setText(QString("Total Encrypted Files: %1").arg(encryptedFilesCount));
  sizeOfDatabaseLabel->setText(QString("Size of Database: %1KB").arg(databaseSize));

  algoCakeDiagram->addValue("AES-128", aes128Count);
  algoCakeDiagram->addValue("AES-256", aes256Count);
  LostFilesCakeDiagram->addValue("Lost Files", lostFilesCount);
  LostFilesCakeDiagram->addValue("Found Files", FoundFilesCount);

  algoFileCounts["AES-128"] = lifetimeAes128Count;
  algoFileCounts["AES-256"] = lifetimeAes256Count;
  algoStatsWidget->update(algoFileCounts);

  saveStatisticsToJson("statistics.json");
}

/**
 * @brief Saves the statistics to a JSON file.
 * @param filePath The path to the JSON file.
 */
void StatisticsScreenWidget::saveStatisticsToJson(const QString &filePath) {
  QJsonObject json;
  QJsonObject lifetimeAlgoJson;
  QJsonObject currentSessionAlgoJson;
  QJsonObject lostFilesJson;

  lifetimeAlgoJson.insert("AES-128", lifetimeAes128Count);
  lifetimeAlgoJson.insert("AES-256", lifetimeAes256Count);
  currentSessionAlgoJson.insert("AES-128", aes128Count);
  currentSessionAlgoJson.insert("AES-256", aes256Count);
  lostFilesJson.insert("Lost Files", lostFilesCount);
  lostFilesJson.insert("Found Files", FoundFilesCount);

  QJsonObject lifetimeStatistics;
  lifetimeStatistics["Algorithms"] = lifetimeAlgoJson;

  QJsonObject currentSessionStatistics;
  currentSessionStatistics["Algorithms"] = currentSessionAlgoJson;
  currentSessionStatistics["DecryptedFilesCount"] = decryptedFilesCount;
  currentSessionStatistics["EncryptedFilesCount"] = encryptedFilesCount;

  json["LifetimeStatistics"] = lifetimeStatistics;
  json["CurrentSessionStatistics"] = currentSessionStatistics;
  json["LostFiles"] = lostFilesJson;
  json["DatabaseSize"] = databaseSize;

  QJsonDocument doc(json);
  QFile file(filePath);
  if (file.open(QIODevice::WriteOnly)) {
    file.write(doc.toJson());
    file.close();
  }
}

/**
 * @brief Loads the statistics from a JSON file.
 * @param filePath The path to the JSON file.
 */
void StatisticsScreenWidget::loadStatisticsFromJson(const QString &filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    return;
  }

  QByteArray data = file.readAll();
  file.close();

  QJsonDocument doc(QJsonDocument::fromJson(data));
  QJsonObject json = doc.object();

  QJsonObject lifetimeStatistics = json["LifetimeStatistics"].toObject();
  QJsonObject currentSessionStatistics = json["CurrentSessionStatistics"].toObject();
  QJsonObject algoJson = currentSessionStatistics["Algorithms"].toObject();
  QJsonObject lostFilesJson = json["LostFiles"].toObject();

  lifetimeAes128Count = lifetimeStatistics["Algorithms"].toObject()["AES-128"].toInt();
  lifetimeAes256Count = lifetimeStatistics["Algorithms"].toObject()["AES-256"].toInt();
  algoFileCounts["AES-128"] = lifetimeAes128Count;
  algoFileCounts["AES-256"] = lifetimeAes256Count;

  aes128Count = algoJson["AES-128"].toInt();
  aes256Count = algoJson["AES-256"].toInt();
  lostFilesCount = lostFilesJson["Lost Files"].toInt();
  FoundFilesCount = lostFilesJson["Found Files"].toInt();

  decryptedFilesCount = currentSessionStatistics["DecryptedFilesCount"].toInt();
  encryptedFilesCount = currentSessionStatistics["EncryptedFilesCount"].toInt();
  databaseSize = json["DatabaseSize"].toInt();

  decryptedFilesLabel->setText(QString("Total Decrypted Files: %1").arg(decryptedFilesCount));
  encryptedFilesLabel->setText(QString("Total Encrypted Files: %1").arg(encryptedFilesCount));
  sizeOfDatabaseLabel->setText(QString("Size of Database: %1MB").arg(databaseSize));
}

/**
 * @brief Ensures that the statistics file exists.
 * @param filePath The path to the statistics file.
 */
void StatisticsScreenWidget::ensureStatisticsFileExists(const QString &filePath) {
  QFile file(filePath);
  if (!file.exists()) {
    QJsonObject json;
    QJsonObject algoJson;
    QJsonObject lostFilesJson;

    algoJson.insert("AES-128", 0);
    algoJson.insert("AES-256", 0);
    lostFilesJson.insert("Lost Files", 0);
    lostFilesJson.insert("Found Files", 0);

    json["LifetimeStatistics"] = QJsonObject{{"Algorithms", algoJson}};
    json["CurrentSessionStatistics"] = QJsonObject{
        {"Algorithms", algoJson},
        {"DecryptedFilesCount", 0},
        {"EncryptedFilesCount", 0}
    };
    json["LostFiles"] = lostFilesJson;
    json["DatabaseSize"] = 0;

    QJsonDocument doc(json);
    if (file.open(QIODevice::WriteOnly)) {
      file.write(doc.toJson());
      file.close();
    }
  }
}

/**
 * @brief Updates the AES-128 file count.
 * @param amount The amount to update by.
 */
void StatisticsScreenWidget::updateAes128Count(int amount) {
  aes128Count += amount;
  if (amount > 0) {
    lifetimeAes128Count += amount;
  }
  updateStatistics();
  saveStatisticsToJson("statistics.json");
}

/**
 * @brief Updates the AES-256 file count.
 * @param amount The amount to update by.
 */
void StatisticsScreenWidget::updateAes256Count(int amount) {
  aes256Count += amount;
  if (amount > 0) {
    lifetimeAes256Count += amount;
  }
  updateStatistics();
  saveStatisticsToJson("statistics.json");
}

/**
 * @brief Updates the lost files count.
 * @param amount The amount to update by.
 */
void StatisticsScreenWidget::updateLostFilesCount(int amount) {
  lostFilesCount += amount;
  updateStatistics();
  saveStatisticsToJson("statistics.json");
}

/**
 * @brief Updates the found files count.
 * @param amount The amount to update by.
 */
void StatisticsScreenWidget::updateFoundFilesCount(int amount) {
  FoundFilesCount += amount;
  updateStatistics();
  saveStatisticsToJson("statistics.json");
}

/**
 * @brief Updates the decrypted files count.
 * @param amount The amount to update by.
 */
void StatisticsScreenWidget::updateDecryptedFilesCount(int amount) {
  decryptedFilesCount += amount;
  updateStatistics();
  saveStatisticsToJson("statistics.json");
}

/**
 * @brief Updates the encrypted files count.
 * @param amount The amount to update by.
 */
void StatisticsScreenWidget::updateEncryptedFilesCount(int amount) {
  encryptedFilesCount += amount;
  updateStatistics();
  saveStatisticsToJson("statistics.json");
}