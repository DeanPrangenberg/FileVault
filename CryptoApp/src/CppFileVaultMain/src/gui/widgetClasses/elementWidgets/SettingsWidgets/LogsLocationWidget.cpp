#include "LogsLocationWidget.h"

LogsLocationWidget::LogsLocationWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("Logs Location");
  setMinimumHeight(130);
  selectLogsLocationButton = std::make_shared<QPushButton>("Select Logs Location", this);
  logsLocationLabel = std::make_unique<QLabel>("Current Path: ", this);

  selectLogsLocationButton->setFixedHeight(30);

  buttons.push_back(selectLogsLocationButton);


  centralLayout->addWidget(selectLogsLocationButton.get());
  centralLayout->addWidget(logsLocationLabel.get());

  applyStyle();
}
