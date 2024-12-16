#include "DatabaseManagementWidget.h"

DatabaseManagementWidget::DatabaseManagementWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  title = std::make_unique<QLabel>("Database Export", this);
  databaseExportComboBox = std::make_shared<QComboBox>(this);
  databaseExportComboBox->addItems({"Encrypted", "Unencrypted"});

  comboBoxes.push_back(databaseExportComboBox);

  centralLayout->addWidget(title.get());
  centralLayout->addWidget(databaseExportComboBox.get());

  applyStyle();
}
