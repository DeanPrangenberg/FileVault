#include "CentralFileStorageWidget.h"

CentralFileStorageWidget::CentralFileStorageWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  CentralFileStorageWidgetLayout = new QVBoxLayout(this);
  title = new QLabel("Central Storage", this);
  enableCentralStorage = new QCheckBox("Enable Central Storage", this);
  useEncryption = new QCheckBox("Use Encryption", this);
  selectStoragePathButton = new QPushButton("Select Storage Path", this);
  storagePathLabel = new QLabel("Current Path: ", this);

  CentralFileStorageWidgetLayout->addWidget(title);
  CentralFileStorageWidgetLayout->addWidget(enableCentralStorage);
  CentralFileStorageWidgetLayout->addWidget(useEncryption);
  CentralFileStorageWidgetLayout->addWidget(selectStoragePathButton);
  CentralFileStorageWidgetLayout->addWidget(storagePathLabel);

  setLayout(CentralFileStorageWidgetLayout);
}

CentralFileStorageWidget::~CentralFileStorageWidget() {
  delete CentralFileStorageWidgetLayout;
}