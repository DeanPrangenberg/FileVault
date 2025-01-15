#ifndef ENCRYPTIONSCREENWIDGET_H
#define ENCRYPTIONSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <memory>
#include <QLabel>
#include <QPushButton>
#include "widgetClasses/elementWidgets/cryptionWidgets/FilePicker.h"
#include "../../../StructUtils/StructUtils.h"
#include "../../../shared/src/GlobalDefinitions.h"
#include "../../../DLLUtils/CryptoDLL.h"
#include "../../../HelperUtils/HelperUtils.h"
#include "StatisticsScreenWidget.h"

class EncryptionScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit EncryptionScreenWidget(QWidget *parent = nullptr, std::shared_ptr<StatisticsScreenWidget> statisticsPtr = nullptr);
  ~EncryptionScreenWidget() override = default;

private:
  StyleSetter StyleSettings;
  std::unique_ptr<QVBoxLayout> EncryptionScreenWidgetLayout;
  std::unique_ptr<QVBoxLayout> EncryptionScreenMainLayout;
  std::unique_ptr<QLabel> EncryptionScreenTitle;
  std::unique_ptr<QPushButton> StartProcessButton;
  std::unique_ptr<FilePicker> FilePickerWidget;

  std::shared_ptr<StatisticsScreenWidget> statisticsScreenWidget;

  void configureUI();

private slots:
  void onStartProcessButtonClicked();
};

#endif //ENCRYPTIONSCREENWIDGET_H