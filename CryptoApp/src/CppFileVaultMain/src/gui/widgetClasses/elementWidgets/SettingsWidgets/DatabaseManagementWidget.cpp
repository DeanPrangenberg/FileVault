#include <QFileDialog>
#include "DatabaseManagementWidget.h"

DatabaseManagementWidget::DatabaseManagementWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("Database Management");
  setMinimumHeight(100);

  restApiDLL = std::make_unique<RestApiDLL>();
  helperUtils = std::make_unique<HelperUtils>();

  hLayout1 = std::make_unique<QHBoxLayout>();
  hLayout2 = std::make_unique<QHBoxLayout>();

  exportButton = std::make_shared<QPushButton>(this);
  exportButton->setText("Export");
  insertButton = std::make_shared<QPushButton>(this);
  insertButton->setText("Insert");
  replaceButton = std::make_shared<QPushButton>(this);
  replaceButton->setText("Replace");
  resetButton = std::make_shared<QPushButton>(this);
  resetButton->setText("Reset");

  hLayout1->addWidget(exportButton.get());
  hLayout1->addWidget(insertButton.get());
  hLayout2->addWidget(replaceButton.get());
  hLayout2->addWidget(resetButton.get());

  buttons.push_back(exportButton);
  buttons.push_back(insertButton);
  buttons.push_back(replaceButton);
  buttons.push_back(resetButton);

  centralLayout->addLayout(hLayout1.release());
  centralLayout->addLayout(hLayout2.release());
  applyStyle();

  connect(exportButton.get(), &QPushButton::clicked, this, &DatabaseManagementWidget::exportDB);
  connect(insertButton.get(), &QPushButton::clicked, this, &DatabaseManagementWidget::insertDB);
  connect(replaceButton.get(), &QPushButton::clicked, this, &DatabaseManagementWidget::replaceDB);
  connect(resetButton.get(), &QPushButton::clicked, this, &DatabaseManagementWidget::resetDB);
}

void DatabaseManagementWidget::exportDB() {
  const auto databaseExport = restApiDLL->ExportDatabase();

  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Database File Location"), "",
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty()) {
    helperUtils->saveDatabaseToFile(fs::path(dir.toStdString()), databaseExport);
  }

}

void DatabaseManagementWidget::insertDB() {
  QString filePath = QFileDialog::getOpenFileName(this, tr("Select Database File Location"), "",
                                                  tr("Database Files (*.FvDB)"));

  qDebug() << "File Path: " << filePath;
  auto DBmap = helperUtils->loadDatabaseFromFile(fs::path(filePath.toStdString()));
  qDebug() << "Got DB map";

  auto data = DBmap.at("Data");
  auto key = DBmap.at("Key");

  qDebug() << "Data: " << data;
  qDebug() << "Key: " << key;

  if (!data.empty() && !key.empty()) {
    if (restApiDLL->InsertDatabase(key, data)) {
      qDebug() << "Database inserted successfully";
    } else {
      qDebug() << "Database insert failed";
    }
  }
}

void DatabaseManagementWidget::replaceDB() {
  QString filePath = QFileDialog::getOpenFileName(this, tr("Select Database File Location"), "",
                                                  tr("Database Files (*.FvDB)"));

  auto DBmap = helperUtils->loadDatabaseFromFile(fs::path(filePath.toStdString()));
  auto data = DBmap.at("Data");
  auto key = DBmap.at("Key");
  if (!data.empty() && !key.empty()) {
    if (restApiDLL->ReplaceDatabase(key, data)) {
      qDebug() << "Database replace successfully";
    } else {
      qDebug() << "Database replace failed";
    }
  }
}

void DatabaseManagementWidget::resetDB() {
  restApiDLL->ResetDatabase();
}
