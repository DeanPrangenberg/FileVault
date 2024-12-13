#include "StandardAlgorithmWidget.h"

StandardAlgorithmWidget::StandardAlgorithmWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  StandardAlgorithmWidgetLayout = new QVBoxLayout(this);
  title = new QLabel("Standard Algorithm", this);
  algorithmComboBox = new QComboBox(this);
  algorithmComboBox->addItems({"AES-128", "AES-256"});

  StandardAlgorithmWidgetLayout->addWidget(title);
  StandardAlgorithmWidgetLayout->addWidget(algorithmComboBox);

  setLayout(StandardAlgorithmWidgetLayout);
}

StandardAlgorithmWidget::~StandardAlgorithmWidget() {
  delete StandardAlgorithmWidgetLayout;
}