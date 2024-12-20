#include "DatabaseManagementWidget.h"

DatabaseManagementWidget::DatabaseManagementWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("Database Export");
  setMinimumHeight(100);
  databaseExportComboBox = std::make_shared<QComboBox>(this);
  databaseExportComboBox->addItems({"Encrypted", "Unencrypted"});

  comboBoxes.push_back(databaseExportComboBox);


  centralLayout->addWidget(databaseExportComboBox.get());

  applyStyle();
}
