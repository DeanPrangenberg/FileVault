#include "SettingsScreenWidget.h"

SettingsScreenWidget::SettingsScreenWidget(QWidget *parent) : QWidget(parent) {
  SettingsScreenWidgetLayout = new QGridLayout(this);

  // Title
  auto *title = new QLabel("Settings", this);
  title->setAlignment(Qt::AlignCenter);
  SettingsScreenWidgetLayout->addWidget(title, 0, 0, 1, 2);

  StyleSetter styleSetter;

  // File Deletion Section
  fileDeletionWidget = new FileDeletionWidget();
  SettingsScreenWidgetLayout->addWidget(fileDeletionWidget, 1, 0);

  // Standard Algorithm Section
  algorithmWidget = new StandardAlgorithmWidget();
  SettingsScreenWidgetLayout->addWidget(algorithmWidget, 1, 1);

  // Logs Location Section
  logsLocationWidget = new LogsLocationWidget();
  SettingsScreenWidgetLayout->addWidget(logsLocationWidget, 2, 0);

  // Set New Password Section
  passwordWidget = new NewPasswordWidget();
  SettingsScreenWidgetLayout->addWidget(passwordWidget, 2, 1);

  // Language Selection Section
  languageWidget = new LanguageSelectionWidget();
  SettingsScreenWidgetLayout->addWidget(languageWidget, 3, 0);

  // Database Export Section
  databaseExportWidget = new DatabaseManagementWidget();
  SettingsScreenWidgetLayout->addWidget(databaseExportWidget, 3, 1);

  // Central Storage Section
  centralStorageWidget = new CentralFileStorageWidget();
  SettingsScreenWidgetLayout->addWidget(centralStorageWidget, 4, 0, 1, 2);

  setLayout(SettingsScreenWidgetLayout);

  // Connect signals to slots
  connect(logsLocationWidget->selectLogsLocationButton, &QPushButton::clicked, this, &SettingsScreenWidget::selectLogsLocation);
  connect(passwordWidget->setPasswordButton, &QPushButton::clicked, this, &SettingsScreenWidget::setPassword);
  connect(centralStorageWidget->selectStoragePathButton, &QPushButton::clicked, this, &SettingsScreenWidget::selectStoragePath);

  // Load settings from file
  loadSettings();
}

void SettingsScreenWidget::selectLogsLocation() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Logs Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Storage Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty()) {
    // Update the label and save the setting
    centralStorageWidget->storagePathLabel->setText("Current Path: " + dir);
    saveSettings();
  }
}

void SettingsScreenWidget::saveSettings() {
  QFile file("settings.txt");
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    // Save settings to file
    out << "DeleteAfterEncryption=" << fileDeletionWidget->deleteAfterEncryption->isChecked() << "\n";
    out << "DeleteAfterDecryption=" << fileDeletionWidget->deleteAfterDecryption->isChecked() << "\n";
    out << "Algorithm=" << algorithmWidget->algorithmComboBox->currentText() << "\n";
    out << "LogsLocation=" << logsLocationWidget->logsLocationLabel->text().remove("Current Path: ") << "\n";
    out << "Language=" << languageWidget->languageComboBox->currentText() << "\n";
    out << "DatabaseExport=" << databaseExportWidget->databaseExportComboBox->currentText() << "\n";
    out << "EnableCentralStorage=" << centralStorageWidget->enableCentralStorage->isChecked() << "\n";
    out << "UseEncryption=" << centralStorageWidget->useEncryption->isChecked() << "\n";
    out << "StoragePath=" << centralStorageWidget->storagePathLabel->text().remove("Current Path: ") << "\n";
    file.close();
  }
}

void SettingsScreenWidget::loadSettings() {
  QFile file("settings.txt");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    // Load settings from file
    while (!in.atEnd()) {
      QString line = in.readLine();
      QStringList parts = line.split("=");
      if (parts.size() == 2) {
        QString key = parts[0];
        QString value = parts[1];
        if (key == "DeleteAfterEncryption") {
          fileDeletionWidget->deleteAfterEncryption->setChecked(value == "1");
        } else if (key == "DeleteAfterDecryption") {
          fileDeletionWidget->deleteAfterDecryption->setChecked(value == "1");
        } else if (key == "Algorithm") {
          algorithmWidget->algorithmComboBox->setCurrentText(value);
        } else if (key == "LogsLocation") {
          logsLocationWidget->logsLocationLabel->setText("Current Path: " + value);
        } else if (key == "Language") {
          languageWidget->languageComboBox->setCurrentText(value);
        } else if (key == "DatabaseExport") {
          databaseExportWidget->databaseExportComboBox->setCurrentText(value);
        } else if (key == "EnableCentralStorage") {
          centralStorageWidget->enableCentralStorage->setChecked(value == "1");
        } else if (key == "UseEncryption") {
          centralStorageWidget->useEncryption->setChecked(value == "1");
        } else if (key == "StoragePath") {
          centralStorageWidget->storagePathLabel->setText("Current Path: " + value);
        }
      }
    }
    file.close();
  }
}

SettingsScreenWidget::~SettingsScreenWidget() {
  delete fileDeletionWidget;
  delete algorithmWidget;
  delete logsLocationWidget;
  delete passwordWidget;
  delete languageWidget;
  delete databaseExportWidget;
  delete centralStorageWidget;
  delete SettingsScreenWidgetLayout;
}