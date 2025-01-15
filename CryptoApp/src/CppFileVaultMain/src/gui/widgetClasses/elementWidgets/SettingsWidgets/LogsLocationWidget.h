#ifndef LOGSLOCATIONWIDGET_H
#define LOGSLOCATIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <functional>
#include "MasterSettingsWidget.h"
#include "../../../shared/src/settingsClasses/Logs.h"

class LogsLocationWidget : public MasterSettingsWidget {
  Q_OBJECT

public:
  explicit LogsLocationWidget(QWidget *parent = nullptr, std::function<void()> ptrUpdateSettings = nullptr);
  std::unique_ptr<QLabel> logsLocationLabel;
  std::shared_ptr<QPushButton> selectLogsLocationButton;

private:
  std::function<void()> updateSettings;
  void updateLogsLocation();
};

#endif // LOGSLOCATIONWIDGET_H