#ifndef SETTINGSSCREENWIDGET_H
#define SETTINGSSCREENWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <memory>
#include "../elementWidgets/SettingsMenu/LanguageSelectionWidget.h"
#include "../elementWidgets/SettingsMenu/StandardAlgorithmWidget.h"
#include "../elementWidgets/SettingsMenu/DatabaseManagementWidget.h"
#include "../elementWidgets/SettingsMenu/FileDeletionWidget.h"
#include "../elementWidgets/SettingsMenu/NewPasswordWidget.h"
#include "../elementWidgets/SettingsMenu/LogsLocationWidget.h"
#include "../elementWidgets/SettingsMenu/CentralFileStorageWidget.h"

class SettingsScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit SettingsScreenWidget(QWidget *parent = nullptr);
  ~SettingsScreenWidget() override = default;

private:
  std::unique_ptr<QLabel> title;
  std::unique_ptr<QGridLayout> SettingsScreenWidgetLayout;
  std::unique_ptr<LanguageSelectionWidget> languageWidget;
  std::unique_ptr<StandardAlgorithmWidget> algorithmWidget;
  std::unique_ptr<DatabaseManagementWidget> databaseExportWidget;
  std::unique_ptr<FileDeletionWidget> fileDeletionWidget;
  std::unique_ptr<NewPasswordWidget> passwordWidget;
  std::unique_ptr<LogsLocationWidget> logsLocationWidget;
  std::unique_ptr<CentralFileStorageWidget> centralStorageWidget;

  void selectLogsLocation();
  void setPassword();
  void selectStoragePath();
  void saveSettings();
  void loadSettings();
};

#endif // SETTINGSSCREENWIDGET_H