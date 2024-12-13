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
  ~LogsLocationWidget() override;

  QLabel *logsLocationLabel;
  QPushButton *selectLogsLocationButton;

private:
  QVBoxLayout *LogsLocationWidgetLayout;
  QLabel *title;
};

#endif // LOGSLOCATIONWIDGET_H