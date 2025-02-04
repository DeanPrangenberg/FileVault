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
#include <qcoreapplication.h>
#include "../../../shared/src/settingsClasses/Logs.h"
#include "../../../shared/src/settingsClasses/CentralFileStorage.h"
#include "../../../shared/src/GlobalDefinitions.h"
#include "../elementWidgets/SettingsWidgets/MasterSettingsWidget.h"
#include "../elementWidgets/SettingsWidgets/LanguageSelectionWidget.h"
#include "../elementWidgets/SettingsWidgets/StandardAlgorithmWidget.h"
#include "../elementWidgets/SettingsWidgets/DatabaseManagementWidget.h"
#include "../elementWidgets/SettingsWidgets/FileDeletionWidget.h"
#include "../elementWidgets/SettingsWidgets/NewPasswordWidget.h"
#include "../elementWidgets/SettingsWidgets/LogsLocationWidget.h"
#include "../elementWidgets/SettingsWidgets/CentralFileStorageWidget.h"
#include "../../PasswordDialog.h"
#include "../DLLUtils/CryptoDLL.h"

/**
 * @class SettingsScreenWidget
 * @brief A widget to manage settings in the FileVault application.
 */
class SettingsScreenWidget : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Constructs the SettingsScreenWidget object.
   * @param parent The parent widget.
   */
  explicit SettingsScreenWidget(QWidget *parent = nullptr);

  /**
   * @brief Creates a file if it does not exist.
   * @param filePath The path to the file.
   */
  void createFileIfNotExists(const QString &filePath);

  /**
   * @brief Destructor for the SettingsScreenWidget class.
   */
  ~SettingsScreenWidget() override = default;

  /**
   * @brief Saves the settings to a file.
   */
  void saveSettings();

private:
  std::unique_ptr<QScrollArea> scrollArea; ///< Scroll area for the settings.
  std::unique_ptr<QGridLayout> SettingsScreenWidgetLayout; ///< Layout for the settings screen widget.
  std::unique_ptr<QLabel> title; ///< Title label.
  std::unique_ptr<LanguageSelectionWidget> languageWidget; ///< Widget for language selection.
  std::unique_ptr<StandardAlgorithmWidget> algorithmWidget; ///< Widget for standard algorithm selection.
  std::unique_ptr<DatabaseManagementWidget> databaseExportWidget; ///< Widget for database management.
  std::unique_ptr<FileDeletionWidget> fileDeletionWidget; ///< Widget for file deletion.
  std::unique_ptr<NewPasswordWidget> passwordWidget; ///< Widget for setting a new passwordHash.
  std::unique_ptr<LogsLocationWidget> logsLocationWidget; ///< Widget for selecting logs location.
  std::unique_ptr<CentralFileStorageWidget> centralStorageWidget; ///< Widget for central file storage.
  std::unique_ptr<QWidget> containerWidget; ///< Container widget.
  std::unique_ptr<QVBoxLayout> mainLayout; ///< Main layout for the settings screen widget.

  /**
   * @brief Loads the settings from a file.
   */
  void loadSettings();
};

#endif // SETTINGSSCREENWIDGET_H