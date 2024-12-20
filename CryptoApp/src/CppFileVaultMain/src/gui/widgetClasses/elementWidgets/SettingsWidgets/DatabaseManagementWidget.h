#ifndef DATABASEMANAGEMENTWIDGET_H
#define DATABASEMANAGEMENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "MasterSettingsWidget.h"

class DatabaseManagementWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit DatabaseManagementWidget(QWidget *parent = nullptr);

  std::shared_ptr<QComboBox>databaseExportComboBox;
};

#endif // DATABASEMANAGEMENTWIDGET_H