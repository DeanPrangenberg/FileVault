#include "SettingsScreenWidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>

SettingsScreenWidget::SettingsScreenWidget(QWidget *parent) : QWidget(parent) {
  SettingsScreenWidgetLayout = std::make_unique<QGridLayout>(this);
  SettingsScreenWidgetLayout->setAlignment(Qt::AlignTop);
  SettingsScreenWidgetLayout->setContentsMargins(0, 0, 0, 0);

  // Title
  title = std::make_unique<QLabel>("Settings", this);
  title->setAlignment(Qt::AlignCenter);
  SettingsScreenWidgetLayout->addWidget(title.get(), 0, 0, 1, 2);

  // Language Selection Section
  languageWidget = std::make_unique<LanguageSelectionWidget>();
  SettingsScreenWidgetLayout->addWidget(languageWidget.get(), 1, 0, 1, 2);

  // Standard Algorithm Section
  algorithmWidget = std::make_unique<StandardAlgorithmWidget>();
  SettingsScreenWidgetLayout->addWidget(algorithmWidget.get(), 2, 1);

  // Database Export Section
  databaseExportWidget = std::make_unique<DatabaseManagementWidget>();
  SettingsScreenWidgetLayout->addWidget(databaseExportWidget.get(), 2, 0);

  // File Deletion Section
  fileDeletionWidget = std::make_unique<FileDeletionWidget>();
  SettingsScreenWidgetLayout->addWidget(fileDeletionWidget.get(), 3, 0);

  // Set std::make_unique<Password Section
  passwordWidget = std::make_unique<NewPasswordWidget>();
  SettingsScreenWidgetLayout->addWidget(passwordWidget.get(), 3, 1);

  // Logs Location Section
  logsLocationWidget = std::make_unique<LogsLocationWidget>();
  SettingsScreenWidgetLayout->addWidget(logsLocationWidget.get(), 4, 0, 1, 2);

  // Central Storage Section
  centralStorageWidget = std::make_unique<CentralFileStorageWidget>();
  SettingsScreenWidgetLayout->addWidget(centralStorageWidget.get(), 5, 0, 1, 2);

  setLayout(SettingsScreenWidgetLayout.get());

  // Connect signals to slots
  connect(logsLocationWidget->selectLogsLocationButton.get(), &QPushButton::clicked, this,
          &SettingsScreenWidget::selectLogsLocation);
  connect(passwordWidget->setPasswordButton.get(), &QPushButton::clicked, this, &SettingsScreenWidget::setPassword);
  connect(centralStorageWidget->selectStoragePathButton.get(), &QPushButton::clicked, this,
          &SettingsScreenWidget::selectStoragePath);

  // Load settings from file
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