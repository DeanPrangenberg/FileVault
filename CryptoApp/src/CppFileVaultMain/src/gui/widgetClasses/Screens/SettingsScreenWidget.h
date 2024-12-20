// In SettingsScreenWidget.h
#ifndef SETTINGSSCREENWIDGET_H
#define SETTINGSSCREENWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <memory>
#include <QScrollArea>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include "../elementWidgets/SettingsWidgets/MasterSettingsWidget.h"
#include "../elementWidgets/SettingsWidgets/LanguageSelectionWidget.h"
#include "../elementWidgets/SettingsWidgets/StandardAlgorithmWidget.h"
#include "../elementWidgets/SettingsWidgets/DatabaseManagementWidget.h"
#include "../elementWidgets/SettingsWidgets/FileDeletionWidget.h"
#include "../elementWidgets/SettingsWidgets/NewPasswordWidget.h"
#include "../elementWidgets/SettingsWidgets/LogsLocationWidget.h"
#include "../elementWidgets/SettingsWidgets/CentralFileStorageWidget.h"

class SettingsScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit SettingsScreenWidget(QWidget *parent = nullptr);
  ~SettingsScreenWidget() override = default;

private:
  std::unique_ptr<QScrollArea> scrollArea;
  std::unique_ptr<QGridLayout> SettingsScreenWidgetLayout;
  std::unique_ptr<QLabel> title;
  std::unique_ptr<LanguageSelectionWidget> languageWidget;
  std::unique_ptr<StandardAlgorithmWidget> algorithmWidget;
  std::unique_ptr<DatabaseManagementWidget> databaseExportWidget;
  std::unique_ptr<FileDeletionWidget> fileDeletionWidget;
  std::unique_ptr<NewPasswordWidget> passwordWidget;
  std::unique_ptr<LogsLocationWidget> logsLocationWidget;
  std::unique_ptr<CentralFileStorageWidget> centralStorageWidget;
  std::unique_ptr<QWidget> containerWidget;
  std::unique_ptr<QVBoxLayout> mainLayout;

  void selectLogsLocation();
  void setPassword();
  void selectStoragePath();
  void saveSettings();
  void loadSettings();
};

#endif // SETTINGSSCREENWIDGET_H