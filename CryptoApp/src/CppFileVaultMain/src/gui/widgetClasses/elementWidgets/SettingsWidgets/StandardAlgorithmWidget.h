#ifndef STANDARDALGORITHMWIDGET_H
#define STANDARDALGORITHMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "MasterSettingsWidget.h"

class StandardAlgorithmWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit StandardAlgorithmWidget(QWidget *parent = nullptr);
  std::shared_ptr<QComboBox> algorithmComboBox;
};

#endif // STANDARDALGORITHMWIDGET_H