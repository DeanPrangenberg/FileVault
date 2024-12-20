#include "StandardAlgorithmWidget.h"

StandardAlgorithmWidget::StandardAlgorithmWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("Standard Algorithm");
  setMinimumHeight(100);
  algorithmComboBox = std::make_shared<QComboBox>(this);
  algorithmComboBox->addItems({"AES-128", "AES-256"});

  comboBoxes.push_back(algorithmComboBox);


  centralLayout->addWidget(algorithmComboBox.get());

  applyStyle();
}
