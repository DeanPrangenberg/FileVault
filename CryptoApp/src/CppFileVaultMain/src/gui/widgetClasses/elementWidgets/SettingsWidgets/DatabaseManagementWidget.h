#ifndef DATABASEMANAGEMENTWIDGET_H
#define DATABASEMANAGEMENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <filesystem>
#include "MasterSettingsWidget.h"
#include "RestAPI/RestApiDLL.h"
#include "../../../../HelperUtils/HelperUtils.h"

namespace fs = std::filesystem;

class DatabaseManagementWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit DatabaseManagementWidget(QWidget *parent = nullptr);
  std::unique_ptr<RestApiDLL> restApiDLL;
  std::unique_ptr<HelperUtils> helperUtils;

  std::unique_ptr<QHBoxLayout> hLayout1;
  std::unique_ptr<QHBoxLayout> hLayout2;
  std::shared_ptr<QPushButton> exportButton;
  std::shared_ptr<QPushButton> insertButton;
  std::shared_ptr<QPushButton> resetButton;
  std::shared_ptr<QPushButton> replaceButton;

  void exportDB();
  void insertDB();
  void replaceDB();
  void resetDB();
};

#endif // DATABASEMANAGEMENTWIDGET_H