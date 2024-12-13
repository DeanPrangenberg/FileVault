#ifndef MASTERSETTINGSWIDGET_H
#define MASTERSETTINGSWIDGET_H

#include <QWidget>
#include "../../../styleClass/StyleSetter.h"

class MasterSettingsWidget : public QWidget {
Q_OBJECT

public:
  explicit MasterSettingsWidget(QWidget *parent = nullptr);
  virtual ~MasterSettingsWidget() {}

protected:
  void showEvent(QShowEvent *event) override;

private:
  void applyStyle(QWidget *widget);
};

#endif // MASTERSETTINGSWIDGET_H