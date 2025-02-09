#include "StandardAlgorithmWidget.h"

/**
 * @brief Constructs a StandardAlgorithmWidget.
 *
 * @param parent The parent widget.
 */
StandardAlgorithmWidget::StandardAlgorithmWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("Standard Algorithm");
  setMinimumHeight(100);

  // Create and initialize the algorithm combo box with options
  algorithmComboBox = std::make_shared<QComboBox>(this);
  algorithmComboBox->addItems({"AES-128", "AES-256"});

  // Add the combo box to the list of combo boxes
  comboBoxes.push_back(algorithmComboBox);

  // Add the combo box to the central layout
  centralLayout->addWidget(algorithmComboBox.get());

  // Apply the predefined style settings to the widget
  applyStyle();
}