#ifndef ENCRYPTIONSCREENWIDGET_H
#define ENCRYPTIONSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <memory>
#include <QLabel>
#include <QPushButton>
#include "../elementWidgets/MasterClasses/FilePicker.h"

class EncryptionScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit EncryptionScreenWidget(QWidget *parent = nullptr);
  ~EncryptionScreenWidget() override = default;
private:
  StyleSetter StyleSettings;
  std::unique_ptr<QVBoxLayout> EncryptionScreenWidgetLayout;
  std::unique_ptr<QVBoxLayout> EncryptionScreenMainLayout;
  std::unique_ptr<QLabel> EncryptionScreenTitle;
  std::unique_ptr<QPushButton> StartProcessButton;
  std::unique_ptr<FilePicker> FilePickerWidget;

  void configureUI();

  void configureFilePicker();
};

#endif //ENCRYPTIONSCREENWIDGET
