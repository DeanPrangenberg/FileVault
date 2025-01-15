#ifndef DECRYPTIONSCREENWIDGET_H
#define DECRYPTIONSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <memory>
#include <QLabel>
#include <QPushButton>
#include <unordered_map>
#include "widgetClasses/elementWidgets/cryptionWidgets/FilePicker.h"
#include "../../../StructUtils/StructUtils.h"
#include "../../../HelperUtils/HelperUtils.h"
#include "../../../shared/src/GlobalDefinitions.h"
#include "../../../DLLUtils/CryptoDLL.h"
#include "StatisticsScreenWidget.h"

class DecryptionScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit DecryptionScreenWidget(QWidget *parent = nullptr, std::shared_ptr<StatisticsScreenWidget> statisticsPtr = nullptr);
  ~DecryptionScreenWidget() override = default;

private:
  StyleSetter StyleSettings;
  std::unique_ptr<QVBoxLayout> DecryptionScreenWidgetLayout;
  std::unique_ptr<QVBoxLayout> DecryptionScreenMainLayout;
  std::unique_ptr<QLabel> DecryptionScreenTitle;
  std::unique_ptr<QPushButton> StartProcessButton;
  std::unique_ptr<FilePicker> FilePickerWidget;

  std::shared_ptr<StatisticsScreenWidget> statisticsScreenWidget;

  void configureUI();

private slots:
  void onStartProcessButtonClicked();
};

#endif //DECRYPTIONSCREENWIDGET_H