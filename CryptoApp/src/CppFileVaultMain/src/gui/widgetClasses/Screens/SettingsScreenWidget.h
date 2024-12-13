#ifndef SETTINGSSCREENWIDGET_H
#define SETTINGSSCREENWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include "../elementWidgets/SettingsMenu/FileDeletionWidget.h"
#include "../elementWidgets/SettingsMenu/StandardAlgorithmWidget.h"
#include "../elementWidgets/SettingsMenu/LogsLocationWidget.h"
#include "../elementWidgets/SettingsMenu/NewPasswordWidget.h"
#include "../elementWidgets/SettingsMenu/LanguageSelectionWidget.h"
#include "../elementWidgets/SettingsMenu/DatabaseManagementWidget.h"
#include "../elementWidgets/SettingsMenu/CentralFileStorageWidget.h"
#include "../../styleClass/StyleSetter.h"

class SettingsScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit SettingsScreenWidget(QWidget *parent = nullptr);
  ~SettingsScreenWidget() override;

private:
  QGridLayout *SettingsScreenWidgetLayout;

  FileDeletionWidget *fileDeletionWidget;
  StandardAlgorithmWidget *algorithmWidget;
  LogsLocationWidget *logsLocationWidget;
  NewPasswordWidget *passwordWidget;
  LanguageSelectionWidget *languageWidget;
  DatabaseManagementWidget *databaseExportWidget;
  CentralFileStorageWidget *centralStorageWidget;

  void selectLogsLocation();
  void setPassword();
  void selectStoragePath();
  void saveSettings();
  void loadSettings();
};

#endif // SETTINGSSCREENWIDGET_H