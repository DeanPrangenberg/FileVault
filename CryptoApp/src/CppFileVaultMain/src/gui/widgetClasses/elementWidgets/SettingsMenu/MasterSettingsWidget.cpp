#include "MasterSettingsWidget.h"

MasterSettingsWidget::MasterSettingsWidget(QWidget *parent) : QWidget(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  auto mainLayout = std::make_unique<QVBoxLayout>(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  backgroundLabel = std::shared_ptr<QLabel>();
  backgroundLabel->setObjectName("MasterSettingsWidget");
  backgroundLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  mainLayout->addWidget(backgroundLabel.get());
  setLayout(mainLayout.release());

  centralLayout = std::make_unique<QVBoxLayout>(backgroundLabel.get());
  centralLayout->setContentsMargins(0, 0, 0, 0);
  centralLayout->setSpacing(0);
}

void MasterSettingsWidget::applyStyle() {
  StyleSetter styleSetter;

  for (auto& button  : buttons) {
    styleSetter.setButtonStyle(button.get());
  }

  for (auto& comboBox : comboBoxes) {
    styleSetter.setComboBoxStyle(comboBox.get());
  }

  for (auto& checkBox : checkBoxes) {
    styleSetter.setCheckBoxStyle(checkBox.get());
  }

  for (auto& lineEdit : lineEdits) {
    styleSetter.setLineEditStyle(lineEdit.get());
  }
}

MasterSettingsWidget::~MasterSettingsWidget() = default;