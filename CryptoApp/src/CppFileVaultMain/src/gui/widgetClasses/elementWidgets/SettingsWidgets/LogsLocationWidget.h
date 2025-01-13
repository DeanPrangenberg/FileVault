#ifndef LOGSLOCATIONWIDGET_H
#define LOGSLOCATIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include "MasterSettingsWidget.h"
#include "../../../shared/settingsClasses/Logs.h"

class LogsLocationWidget : public MasterSettingsWidget {
  Q_OBJECT

public:
  explicit LogsLocationWidget(QWidget *parent = nullptr);
  std::unique_ptr<QLabel> logsLocationLabel;
  std::shared_ptr<QPushButton> selectLogsLocationButton;

private:
  void updateLogsLocation();

};

#endif // LOGSLOCATIONWIDGET_H
