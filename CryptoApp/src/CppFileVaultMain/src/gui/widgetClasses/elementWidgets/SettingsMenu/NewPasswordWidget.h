#ifndef NEWPASSWORDWIDGET_H
#define NEWPASSWORDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "MasterSettingsWidget.h"

class NewPasswordWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit NewPasswordWidget(QWidget *parent = nullptr);
  ~NewPasswordWidget() override;

  QLineEdit *oldPasswordInput;
  QLineEdit *newPasswordInput;
  QPushButton *setPasswordButton;

private:
  QVBoxLayout *NewPasswordWidgetLayout;
  QLabel *title;
};

#endif // NEWPASSWORDWIDGET_H