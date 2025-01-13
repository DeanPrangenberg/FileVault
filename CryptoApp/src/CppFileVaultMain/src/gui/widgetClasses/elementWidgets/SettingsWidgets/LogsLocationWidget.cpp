#include "LogsLocationWidget.h"

LogsLocationWidget::LogsLocationWidget(QWidget *parent, std::function<void()> ptrUpdateSettings)
  : MasterSettingsWidget(parent), updateSettings(ptrUpdateSettings) {
  titleLabel->setText("Logs Location");
  setMinimumHeight(130);
  selectLogsLocationButton = std::make_shared<QPushButton>("Select Logs Location", this);
  logsLocationLabel = std::make_unique<QLabel>("Current Path: " + QString::fromStdWString(Logs::getLogsDirectory()), this);

  selectLogsLocationButton->setFixedHeight(30);

  buttons.push_back(selectLogsLocationButton);

  centralLayout->addWidget(selectLogsLocationButton.get());
  centralLayout->addWidget(logsLocationLabel.get());

  connect(selectLogsLocationButton.get(), &QPushButton::clicked, this, &LogsLocationWidget::updateLogsLocation, Qt::UniqueConnection);

  applyStyle();
}

void LogsLocationWidget::updateLogsLocation() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Logs Directory"), "",
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty()) {
    logsLocationLabel->setText("Current Path: " + dir);
    Logs::moveLogsDirectory(dir.toStdString());
    Logs::writeToInfoLog("Moved logs directory to: " + dir.toStdString());
    if (updateSettings) {
      updateSettings();
    }
  }
}