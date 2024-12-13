#ifndef CENTRALFILESTORAGEWIDGET_H
#define CENTRALFILESTORAGEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include "MasterSettingsWidget.h"

class CentralFileStorageWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit CentralFileStorageWidget(QWidget *parent = nullptr);
  ~CentralFileStorageWidget() override;

  QCheckBox *enableCentralStorage;
  QCheckBox *useEncryption;
  QPushButton *selectStoragePathButton;
  QLabel *storagePathLabel;

private:
  QVBoxLayout *CentralFileStorageWidgetLayout;
  QLabel *title;

};

#endif // CENTRALFILESTORAGEWIDGET_H