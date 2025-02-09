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

/**
 * @class LogsLocationWidget
 * @brief A widget for selecting and displaying the logs location.
 *
 * This widget allows the user to select a directory for storing logs and displays the current logs path.
 */
class LogsLocationWidget : public MasterSettingsWidget {
  Q_OBJECT

public:
  /**
   * @brief Constructs a LogsLocationWidget.
   *
   * @param parent The parent widget.
   * @param ptrUpdateSettings A function pointer to update settings.
   */
  explicit LogsLocationWidget(QWidget *parent = nullptr, std::function<void()> ptrUpdateSettings = nullptr);

  /// Label to display the current logs location.
  std::unique_ptr<QLabel> logsLocationLabel;

  /// Button to open a dialog for selecting the logs location.
  std::shared_ptr<QPushButton> selectLogsLocationButton;

private:
  /// Function pointer to update settings.
  std::function<void()> updateSettings;

  /**
   * @brief Updates the logs location.
   *
   * This function opens a file dialog to select a new logs directory.
   * If a directory is selected, it updates the logs location label,
   * moves the logs directory, writes to the info log, and calls the updateSettings function.
   */
  void updateLogsLocation();
};

#endif // LOGSLOCATIONWIDGET_H