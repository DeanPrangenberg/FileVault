#ifndef FILEDELETIONWIDGET_H
#define FILEDELETIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include "MasterSettingsWidget.h"

class FileDeletionWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit FileDeletionWidget(QWidget *parent = nullptr);

  std::shared_ptr<QCheckBox> deleteAfterEncryption;
  std::shared_ptr<QCheckBox> deleteAfterDecryption;
};

#endif // FILEDELETIONWIDGET_H