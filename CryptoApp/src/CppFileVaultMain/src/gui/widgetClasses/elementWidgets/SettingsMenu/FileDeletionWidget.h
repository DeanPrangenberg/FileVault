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

  QCheckBox *deleteAfterEncryption;
  QCheckBox *deleteAfterDecryption;

private:
  QVBoxLayout *layout;
  QLabel *title;
};

#endif // FILEDELETIONWIDGET_H