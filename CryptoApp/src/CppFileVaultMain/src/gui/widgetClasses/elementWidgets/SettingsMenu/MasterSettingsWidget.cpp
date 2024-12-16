#include "MasterSettingsWidget.h"

MasterSettingsWidget::MasterSettingsWidget(QWidget *parent) : QWidget(parent) {
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  auto mainLayout = std::make_unique<QVBoxLayout>(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  backgroundLabel = std::make_shared<QLabel>(this);
  backgroundLabel->setObjectName("MasterSettingsWidget");
  backgroundLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  mainLayout->addWidget(backgroundLabel.get());
  setLayout(mainLayout.release());

  centralLayout = std::make_unique<QVBoxLayout>(backgroundLabel.get());
  centralLayout->setContentsMargins(5, 5, 5, 5);
  centralLayout->setSpacing(5);
}

void MasterSettingsWidget::applyStyle() {
  StyleSetter styleSetter;
  styleSetter.setWidgetBackgroundStyle(backgroundLabel);

  for (auto& button  : buttons) {
    styleSetter.setButtonStyle(button);
  }

  for (auto& comboBox : comboBoxes) {
    styleSetter.setComboBoxStyle(comboBox);
  }

  for (auto& checkBox : checkBoxes) {
    styleSetter.setCheckBoxStyle(checkBox);
  }

  for (auto& lineEdit : lineEdits) {
    styleSetter.setLineEditStyle(lineEdit);
  }

  adjustSize();
}

MasterSettingsWidget::~MasterSettingsWidget() = default;