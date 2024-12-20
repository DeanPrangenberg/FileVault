#include "MasterSettingsWidget.h"

MasterSettingsWidget::MasterSettingsWidget(QWidget *parent) : QWidget(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  auto mainLayout = std::make_unique<QVBoxLayout>(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  backgroundLabel = std::make_shared<QLabel>(this);
  backgroundLabel->setObjectName("MasterSettingsWidget");
  backgroundLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  titleLabel = std::make_unique<QLabel>(this);

  mainLayout->addWidget(backgroundLabel.get());
  setLayout(mainLayout.release());

  centralLayout = std::make_unique<QVBoxLayout>(backgroundLabel.get());
  centralLayout->setContentsMargins(5, 5, 5, 5);
  centralLayout->setSpacing(5);
  centralLayout->addWidget(titleLabel.get());

  // Initialisieren Sie die Container
  buttons = QVector<std::shared_ptr<QPushButton>>();
  comboBoxes = QVector<std::shared_ptr<QComboBox>>();
  checkBoxes = QVector<std::shared_ptr<QCheckBox>>();
  lineEdits = QVector<std::shared_ptr<QLineEdit>>();
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
}

MasterSettingsWidget::~MasterSettingsWidget() = default;