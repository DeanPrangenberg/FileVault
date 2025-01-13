#ifndef MASTERSETTINGSWIDGET_H
#define MASTERSETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <memory>
#include "../../../styleClass/StyleSetter.h"

class MasterSettingsWidget : public QWidget {
Q_OBJECT

public:
  explicit MasterSettingsWidget(QWidget *parent = nullptr);
  ~MasterSettingsWidget() override;

protected:
  std::shared_ptr<QLabel> backgroundLabel;
  std::unique_ptr<QVBoxLayout> centralLayout;
  std::unique_ptr<QLabel> titleLabel;
  QVector<std::shared_ptr<QPushButton>> buttons;
  QVector<std::shared_ptr<QComboBox>> comboBoxes;
  QVector<std::shared_ptr<QCheckBox>> checkBoxes;
  QVector<std::shared_ptr<QLineEdit>> lineEdits;
  QVector<std::shared_ptr<QLineEdit>> passwordFields;
  QVector<std::shared_ptr<QPushButton>> passwordToggleButtons;

  void applyStyle();
};

#endif // MASTERSETTINGSWIDGET_H