#ifndef DECRYPTIONSCREENWIDGET_H
#define DECRYPTIONSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <memory>
#include <QLabel>
#include <QPushButton>
#include "widgetClasses/elementWidgets/smallElements/FilePicker.h"
#include "../../../StructUtils/StructUtils.h"
#include "../../../HelperUtils/HelperUtils.h"
#include "../../../GlobalDefinitions.h"
#include "../../../DLLUtils/CryptoDLL.h"

class DecryptionScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit DecryptionScreenWidget(QWidget *parent = nullptr);
  ~DecryptionScreenWidget() override = default;

private:
  StyleSetter StyleSettings;
  std::unique_ptr<QVBoxLayout> DecryptionScreenWidgetLayout;
  std::unique_ptr<QVBoxLayout> DecryptionScreenMainLayout;
  std::unique_ptr<QLabel> DecryptionScreenTitle;
  std::unique_ptr<QPushButton> StartProcessButton;
  std::unique_ptr<FilePicker> FilePickerWidget;

  void configureUI();

private slots:
  void onStartProcessButtonClicked();
};

#endif //DECRYPTIONSCREENWIDGET_H