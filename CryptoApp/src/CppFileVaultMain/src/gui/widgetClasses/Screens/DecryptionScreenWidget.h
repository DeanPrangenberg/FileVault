#ifndef DECRYPTIONSCREENWIDGET_H
#define DECRYPTIONSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <memory>
#include <QLabel>
#include <QPushButton>
#include <unordered_map>
#include "widgetClasses/elementWidgets/cryptionWidgets/FilePicker.h"
#include "../../../AttributeUtils/AttributeUtils.h"
#include "../../../HelperUtils/HelperUtils.h"
#include "../../../shared/src/GlobalDefinitions.h"
#include "../../../DLLUtils/CryptoDLL.h"
#include "StatisticsScreenWidget.h"

/**
 * @class DecryptionScreenWidget
 * @brief A widget to manage the decryption process in the FileVault application.
 */
class DecryptionScreenWidget : public QWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs the DecryptionScreenWidget object.
   * @param parent The parent widget.
   * @param statisticsPtr A shared pointer to the StatisticsScreenWidget.
   */
  explicit DecryptionScreenWidget(QWidget *parent = nullptr, std::shared_ptr<StatisticsScreenWidget> statisticsPtr = nullptr);

  /**
   * @brief Destructor for the DecryptionScreenWidget class.
   */
  ~DecryptionScreenWidget() override = default;

private:
  StyleSetter StyleSettings; ///< Style settings for the widget.
  std::unique_ptr<QVBoxLayout> DecryptionScreenWidgetLayout; ///< Layout for the decryption screen widget.
  std::unique_ptr<QVBoxLayout> DecryptionScreenMainLayout; ///< Main layout for the decryption screen.
  std::unique_ptr<QLabel> DecryptionScreenTitle; ///< Title label for the decryption screen.
  std::unique_ptr<QPushButton> StartProcessButton; ///< Button to start the decryption process.
  std::unique_ptr<FilePicker> FilePickerWidget; ///< Widget to pick files for decryption.

  std::shared_ptr<StatisticsScreenWidget> statisticsScreenWidget; ///< Shared pointer to the statistics screen widget.

  /**
   * @brief Configures the user interface for the decryption screen.
   */
  void configureUI();

private slots:
  /**
   * @brief Slot for handling the start process button click.
   */
  void onStartProcessButtonClicked();
};

#endif //DECRYPTIONSCREENWIDGET_H