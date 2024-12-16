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

  std::shared_ptr<QCheckBox> forEncryptedFiles;
  std::shared_ptr<QCheckBox> forDecryptedFiles;
  std::shared_ptr<QPushButton> selectStoragePathButton;
  std::shared_ptr<QLabel> storagePathLabel;

private:
  std::unique_ptr<QLabel> title;
};

#endif // CENTRALFILESTORAGEWIDGET_H