#include "DatabaseManagementWidget.h"

DatabaseManagementWidget::DatabaseManagementWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  DatabaseManagementWidgetLayout = new QVBoxLayout(this);
  title = new QLabel("Database Export", this);
  databaseExportComboBox = new QComboBox(this);
  databaseExportComboBox->addItems({"Encrypted", "Unencrypted"});

  DatabaseManagementWidgetLayout->addWidget(title);
  DatabaseManagementWidgetLayout->addWidget(databaseExportComboBox);

  setLayout(DatabaseManagementWidgetLayout);
}

DatabaseManagementWidget::~DatabaseManagementWidget() {
  delete DatabaseManagementWidgetLayout;
}