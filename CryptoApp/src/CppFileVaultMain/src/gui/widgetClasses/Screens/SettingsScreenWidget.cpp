#include "SettingsScreenWidget.h"

SettingsScreenWidget::SettingsScreenWidget(QWidget *parent) : QWidget(parent) {
  qDebug() << "SettingsScreenWidget: Creating SettingsScreenWidget";

  // Scroll Area setup
  scrollArea = std::make_unique<QScrollArea>(this);
  scrollArea->setWidgetResizable(true);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
  scrollArea->setContentsMargins(0, 0, 0, 0);

  // Apply styling to scroll area
  StyleSetter styleSetter;
  styleSetter.setScrollAreaStyle(scrollArea.get());

  // Container widget inside the scroll area
  containerWidget = std::make_unique<QWidget>(scrollArea.get());
  containerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  containerWidget->setObjectName("SettingsScreenWidget");
  containerWidget->setStyleSheet(QString("QWidget#%1 { background: Transparent; }")
    .arg(containerWidget->objectName()));

  // Layout for the container widget
  SettingsScreenWidgetLayout = std::make_unique<QGridLayout>(containerWidget.get());
  SettingsScreenWidgetLayout->setContentsMargins(10, 10, 10, 10);
  SettingsScreenWidgetLayout->setSpacing(10);

  // Add widgets to the layout
  title = std::make_unique<QLabel>("Settings", containerWidget.get());
  title->setAlignment(Qt::AlignCenter);
  title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  SettingsScreenWidgetLayout->addWidget(title.get(), 0, 0, 1, 2);

  languageWidget = std::make_unique<LanguageSelectionWidget>(containerWidget.get());
  languageWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  SettingsScreenWidgetLayout->addWidget(languageWidget.get(), 1, 0, 1, 2);
  qDebug() << "SettingsScreenWidget: LanguageSelectionWidget added";

  algorithmWidget = std::make_unique<StandardAlgorithmWidget>(containerWidget.get());
  algorithmWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  SettingsScreenWidgetLayout->addWidget(algorithmWidget.get(), 2, 1);

  databaseExportWidget = std::make_unique<DatabaseManagementWidget>(containerWidget.get());
  databaseExportWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  SettingsScreenWidgetLayout->addWidget(databaseExportWidget.get(), 2, 0);

  fileDeletionWidget = std::make_unique<FileDeletionWidget>(containerWidget.get());
  fileDeletionWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  SettingsScreenWidgetLayout->addWidget(fileDeletionWidget.get(), 3, 0);

  passwordWidget = std::make_unique<NewPasswordWidget>(containerWidget.get());
  passwordWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  SettingsScreenWidgetLayout->addWidget(passwordWidget.get(), 3, 1);

  logsLocationWidget = std::make_unique<LogsLocationWidget>(containerWidget.get(),
                                                            [this] { saveSettings(); });
  logsLocationWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  SettingsScreenWidgetLayout->addWidget(logsLocationWidget.get(), 4, 0, 1, 2);

  centralStorageWidget = std::make_unique<CentralFileStorageWidget>(containerWidget.get());
  centralStorageWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  SettingsScreenWidgetLayout->addWidget(centralStorageWidget.get(), 5, 0, 1, 2);

  // Set the layout for the container widget
  containerWidget->setLayout(SettingsScreenWidgetLayout.get());
  containerWidget->setMinimumSize(SettingsScreenWidgetLayout->sizeHint().width(),
                                  SettingsScreenWidgetLayout->sizeHint().height());


  // Add the container widget to the scroll area
  scrollArea->setWidget(containerWidget.get());

  // Main layout
  mainLayout = std::make_unique<QVBoxLayout>(this);
  mainLayout->addWidget(scrollArea.get());
  mainLayout->setContentsMargins(0, 0, 0, 0);
  setLayout(mainLayout.get());

  connect(fileDeletionWidget->deleteAfterEncryption.get(), &QCheckBox::stateChanged, [this](const int state) {
    globalDefinitions::deleteFileAfterEncryption = (state == Qt::Checked);
    saveSettings();
  });

  connect(fileDeletionWidget->deleteAfterDecryption.get(), &QCheckBox::stateChanged, [this](const int state) {
    globalDefinitions::deleteFileAfterDecryption = (state == Qt::Checked);
    saveSettings();
  });

  // Load settings
  loadSettings();
}

void SettingsScreenWidget::createFileIfNotExists(const QString &filePath) {
  QFile file(filePath);
  if (!file.exists()) {
    if (file.open(QIODevice::WriteOnly)) {
      QJsonObject defaultSettings;
      defaultSettings["DeleteAfterEncryption"] = false;
      defaultSettings["DeleteAfterDecryption"] = false;
      defaultSettings["Algorithm"] = "AES-128";
      defaultSettings["LogsLocation"] = QCoreApplication::applicationDirPath() + "/logs";
      defaultSettings["Language"] = "English";
      defaultSettings["ForEncryptedFiles"] = false;
      defaultSettings["ForDecryptedFiles"] = false;
      defaultSettings["StoragePath"] = QCoreApplication::applicationDirPath() + "/centralStorage";

      QJsonDocument doc(defaultSettings);
      file.write(doc.toJson());
      file.close();
    }
  }
}

void SettingsScreenWidget::saveSettings() {
  QJsonObject settings;
  settings["DeleteAfterEncryption"] = fileDeletionWidget->deleteAfterEncryption->isChecked();
  settings["DeleteAfterDecryption"] = fileDeletionWidget->deleteAfterDecryption->isChecked();
  settings["Algorithm"] = algorithmWidget->algorithmComboBox->currentText();
  settings["LogsLocation"] = logsLocationWidget->logsLocationLabel->text().remove("Current Path: ");
  settings["Language"] = languageWidget->languageComboBox->currentText();
  settings["ForEncryptedFiles"] = centralStorageWidget->forEncryptedFiles->isChecked();
  settings["ForDecryptedFiles"] = centralStorageWidget->forDecryptedFiles->isChecked();
  settings["StoragePath"] = centralStorageWidget->storagePathLabel->text().remove("Current Path: ");

  const QJsonDocument doc(settings);
  const QString filePath = "settings.json";
  createFileIfNotExists(filePath);
  QFile file(filePath);
  if (file.open(QIODevice::WriteOnly)) {
    file.write(doc.toJson());
    file.close();
  }
}

void SettingsScreenWidget::loadSettings() {
  QString filePath = "settings.json";
  createFileIfNotExists(filePath);
  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject settings = doc.object();

    fileDeletionWidget->deleteAfterEncryption->setChecked(settings["DeleteAfterEncryption"].toBool());
    fileDeletionWidget->deleteAfterDecryption->setChecked(settings["DeleteAfterDecryption"].toBool());
    algorithmWidget->algorithmComboBox->setCurrentText(settings["Algorithm"].toString());
    languageWidget->languageComboBox->setCurrentText(settings["Language"].toString());
    centralStorageWidget->forEncryptedFiles->setChecked(settings["ForEncryptedFiles"].toBool());
    centralStorageWidget->forDecryptedFiles->setChecked(settings["ForDecryptedFiles"].toBool());
    centralStorageWidget->storagePathLabel->setText("Current Path: " + settings["StoragePath"].toString());

    if (settings["DeleteAfterEncryption"].toBool()) {
      globalDefinitions::deleteFileAfterEncryption = true;
    }

    if (settings["DeleteAfterDecryption"].toBool()) {
      globalDefinitions::deleteFileAfterDecryption = true;
    }

    if (!settings["LogsLocation"].toString().isEmpty()) {
      Logs::moveLogsDirectory(settings["LogsLocation"].toString().toStdWString());
      logsLocationWidget->logsLocationLabel->setText("Current Path: " + settings["LogsLocation"].toString());
    }

    file.close();
  }
}
