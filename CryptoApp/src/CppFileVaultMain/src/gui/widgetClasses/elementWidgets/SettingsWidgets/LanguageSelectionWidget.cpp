#include "LanguageSelectionWidget.h"

LanguageSelectionWidget::LanguageSelectionWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  titleLabel->setText("Language Selection");
  setMinimumHeight(100);

  languageComboBox = std::make_shared<QComboBox>(this);
  languageComboBox->addItems({"English", "German"});

  comboBoxes.push_back(languageComboBox);


  centralLayout->addWidget(languageComboBox.get());

  applyStyle();
}