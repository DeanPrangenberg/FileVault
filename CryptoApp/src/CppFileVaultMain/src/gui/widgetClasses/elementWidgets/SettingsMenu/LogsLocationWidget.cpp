#include "LogsLocationWidget.h"

LogsLocationWidget::LogsLocationWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  LogsLocationWidgetLayout = new QVBoxLayout(this);
  title = new QLabel("Logs Location", this);
  selectLogsLocationButton = new QPushButton("Select Logs Location", this);
  logsLocationLabel = new QLabel("Current Path: ", this);

  LogsLocationWidgetLayout->addWidget(title);
  LogsLocationWidgetLayout->addWidget(selectLogsLocationButton);
  LogsLocationWidgetLayout->addWidget(logsLocationLabel);

  setLayout(LogsLocationWidgetLayout);
}

LogsLocationWidget::~LogsLocationWidget() {
  delete LogsLocationWidgetLayout;
}