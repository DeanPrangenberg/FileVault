#include "StandardAlgorithmWidget.h"

StandardAlgorithmWidget::StandardAlgorithmWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  title = std::make_unique<QLabel>("Standard Algorithm", this);
  algorithmComboBox = std::make_shared<QComboBox>(this);
  algorithmComboBox->addItems({"AES-128", "AES-256"});

  comboBoxes.push_back(algorithmComboBox);

  centralLayout->addWidget(title.get());
  centralLayout->addWidget(algorithmComboBox.get());

  applyStyle();
}
