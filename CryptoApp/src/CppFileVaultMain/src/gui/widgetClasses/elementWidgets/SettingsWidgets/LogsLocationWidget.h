#ifndef LOGSLOCATIONWIDGET_H
#define LOGSLOCATIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "MasterSettingsWidget.h"

class LogsLocationWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit LogsLocationWidget(QWidget *parent = nullptr);

  std::unique_ptr<QLabel> logsLocationLabel;
  std::shared_ptr<QPushButton> selectLogsLocationButton;
};

#endif // LOGSLOCATIONWIDGET_H