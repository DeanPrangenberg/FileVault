#ifndef ENCRYPTIONSCREENWIDGET_H
#define ENCRYPTIONSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <memory>
#include <QLabel>
#include <QPushButton>
#include "widgetClasses/elementWidgets/cryptionWidgets/FilePicker.h"
#include "../../../AttributeUtils/AttributeUtils.h"
#include "../../../shared/src/GlobalDefinitions.h"
#include "../../../DLLUtils/CryptoDLL.h"
#include "../../../HelperUtils/HelperUtils.h"
#include "StatisticsScreenWidget.h"

/**
 * @class EncryptionScreenWidget
 * @brief A widget to manage the encryption process in the FileVault application.
 */
class EncryptionScreenWidget : public QWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs the EncryptionScreenWidget object.
   * @param parent The parent widget.
   * @param statisticsPtr A shared pointer to the StatisticsScreenWidget.
   */
  explicit EncryptionScreenWidget(QWidget *parent = nullptr, std::shared_ptr<StatisticsScreenWidget> statisticsPtr = nullptr);

  /**
   * @brief Destructor for the EncryptionScreenWidget class.
   */
  ~EncryptionScreenWidget() override = default;

private:
  StyleSetter StyleSettings; ///< Style settings for the widget.
  std::unique_ptr<QVBoxLayout> EncryptionScreenWidgetLayout; ///< Layout for the encryption screen widget.
  std::unique_ptr<QVBoxLayout> EncryptionScreenMainLayout; ///< Main layout for the encryption screen.
  std::unique_ptr<QLabel> EncryptionScreenTitle; ///< Title label for the encryption screen.
  std::unique_ptr<QPushButton> StartProcessButton; ///< Button to start the encryption process.
  std::unique_ptr<FilePicker> FilePickerWidget; ///< Widget to pick files for encryption.

  std::shared_ptr<StatisticsScreenWidget> statisticsScreenWidget; ///< Shared pointer to the statistics screen widget.

  /**
   * @brief Configures the user interface for the encryption screen.
   */
  void configureUI();

private slots:
  /**
   * @brief Slot for handling the start process button click.
   */
  void onStartProcessButtonClicked();
};

#endif //ENCRYPTIONSCREENWIDGET_H