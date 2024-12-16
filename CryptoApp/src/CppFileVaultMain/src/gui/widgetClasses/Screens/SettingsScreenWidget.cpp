#include "SettingsScreenWidget.h"

SettingsScreenWidget::SettingsScreenWidget(QWidget *parent) : QWidget(parent) {
  qDebug() << "SettingsScreenWidget: Creating SettingsScreenWidget";

  scrollArea = std::make_unique<QScrollArea>(this);
  scrollArea->setWidgetResizable(true);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scrollArea->setContentsMargins(0, 0, 0, 0);

  StyleSetter styleSetter;
  styleSetter.setScrollAreaStyle(scrollArea.get());

  containerWidget = std::make_unique<QWidget>(scrollArea.get());
  containerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  containerWidget->setObjectName("SettingsScreenWidget");
  containerWidget->setStyleSheet(QString("QWidget#%1 {"
                                         "background: Transparent;"
                                         "}"
  ).arg(containerWidget->objectName()));
  scrollArea->setWidget(containerWidget.get());

  SettingsScreenWidgetLayout = std::make_unique<QGridLayout>(containerWidget.get());

  SettingsScreenWidgetLayout->setRowStretch(0, 1);
  SettingsScreenWidgetLayout->setRowStretch(1, 4);
  SettingsScreenWidgetLayout->setRowStretch(2, 4);
  SettingsScreenWidgetLayout->setRowStretch(3, 5);
  SettingsScreenWidgetLayout->setRowStretch(4, 4);
  SettingsScreenWidgetLayout->setRowStretch(5, 5);

  SettingsScreenWidgetLayout->setColumnStretch(0, 1);
  SettingsScreenWidgetLayout->setColumnStretch(1, 1);

  title = std::make_unique<QLabel>("Settings", containerWidget.get());
  title->setAlignment(Qt::AlignCenter);
  SettingsScreenWidgetLayout->addWidget(title.get(), 0, 0, 1, 2);

  languageWidget = std::make_unique<LanguageSelectionWidget>(containerWidget.get());
  SettingsScreenWidgetLayout->addWidget(languageWidget.get(), 1, 0, 1, 2);

  algorithmWidget = std::make_unique<StandardAlgorithmWidget>(containerWidget.get());
  SettingsScreenWidgetLayout->addWidget(algorithmWidget.get(), 2, 1);

  databaseExportWidget = std::make_unique<DatabaseManagementWidget>(containerWidget.get());
  SettingsScreenWidgetLayout->addWidget(databaseExportWidget.get(), 2, 0);

  fileDeletionWidget = std::make_unique<FileDeletionWidget>(containerWidget.get());
  SettingsScreenWidgetLayout->addWidget(fileDeletionWidget.get(), 3, 0);

  passwordWidget = std::make_unique<NewPasswordWidget>(containerWidget.get());
  SettingsScreenWidgetLayout->addWidget(passwordWidget.get(), 3, 1);

  logsLocationWidget = std::make_unique<LogsLocationWidget>(containerWidget.get());
  SettingsScreenWidgetLayout->addWidget(logsLocationWidget.get(), 4, 0, 1, 2);

  centralStorageWidget = std::make_unique<CentralFileStorageWidget>(containerWidget.get());
  SettingsScreenWidgetLayout->addWidget(centralStorageWidget.get(), 5, 0, 1, 2);

  containerWidget->setLayout(SettingsScreenWidgetLayout.get());

  mainLayout = std::make_unique<QVBoxLayout>(this);
  mainLayout->addWidget(scrollArea.get());
  setLayout(mainLayout.get());

  connect(logsLocationWidget->selectLogsLocationButton.get(), &QPushButton::clicked, this,
          &SettingsScreenWidget::selectLogsLocation);
  connect(passwordWidget->setPasswordButton.get(), &QPushButton::clicked, this, &SettingsScreenWidget::setPassword);
  connect(centralStorageWidget->selectStoragePathButton.get(), &QPushButton::clicked, this,
          &SettingsScreenWidget::selectStoragePath);

  loadSettings();
}


void SettingsScreenWidget::selectLogsLocation() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Logs Directory"), "",
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty()) {
    // Update the label and save the setting
    logsLocationWidget->logsLocationLabel->setText("Current Path: " + dir);
    saveSettings();
  }
}

void SettingsScreenWidget::setPassword() {
  // Implement password setting logic here
  saveSettings();
}

void SettingsScreenWidget::selectStoragePath() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Storage Directory"), "",
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty()) {
    // Update the label and save the setting
    centralStorageWidget->storagePathLabel->setText("Current Path: " + dir);
    saveSettings();
  }
}

void SettingsScreenWidget::saveSettings() {
  QJsonObject settings;
  settings["DeleteAfterEncryption"] = fileDeletionWidget->deleteAfterEncryption->isChecked();
  settings["DeleteAfterDecryption"] = fileDeletionWidget->deleteAfterDecryption->isChecked();
  settings["Algorithm"] = algorithmWidget->algorithmComboBox->currentText();
  settings["LogsLocation"] = logsLocationWidget->logsLocationLabel->text().remove("Current Path: ");
  settings["Language"] = languageWidget->languageComboBox->currentText();
  settings["DatabaseExport"] = databaseExportWidget->databaseExportComboBox->currentText();
  settings["ForEncryptedFiles"] = centralStorageWidget->forEncryptedFiles->isChecked();
  settings["ForDecryptedFiles"] = centralStorageWidget->forDecryptedFiles->isChecked();
  settings["StoragePath"] = centralStorageWidget->storagePathLabel->text().remove("Current Path: ");

  QJsonDocument doc(settings);
  QFile file("settings.json");
  if (file.open(QIODevice::WriteOnly)) {
    file.write(doc.toJson());
    file.close();
  }
}

void SettingsScreenWidget::loadSettings() {
  QFile file("settings.json");
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject settings = doc.object();

    fileDeletionWidget->deleteAfterEncryption->setChecked(settings["DeleteAfterEncryption"].toBool());
    fileDeletionWidget->deleteAfterDecryption->setChecked(settings["DeleteAfterDecryption"].toBool());
    algorithmWidget->algorithmComboBox->setCurrentText(settings["Algorithm"].toString());
    logsLocationWidget->logsLocationLabel->setText("Current Path: " + settings["LogsLocation"].toString());
    languageWidget->languageComboBox->setCurrentText(settings["Language"].toString());
    databaseExportWidget->databaseExportComboBox->setCurrentText(settings["DatabaseExport"].toString());
    centralStorageWidget->forEncryptedFiles->setChecked(settings["ForEncryptedFiles"].toBool());
    centralStorageWidget->forDecryptedFiles->setChecked(settings["ForDecryptedFiles"].toBool());
    centralStorageWidget->storagePathLabel->setText("Current Path: " + settings["StoragePath"].toString());

    file.close();
  }
}