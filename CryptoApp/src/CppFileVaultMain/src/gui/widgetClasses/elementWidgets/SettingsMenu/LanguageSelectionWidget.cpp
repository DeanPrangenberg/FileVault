#include "LanguageSelectionWidget.h"

LanguageSelectionWidget::LanguageSelectionWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  title = std::make_unique<QLabel>("Language Selection", this);

  languageComboBox = std::make_shared<QComboBox>(this);
  languageComboBox->addItems({"English", "German"});

  comboBoxes.push_back(languageComboBox);

  centralLayout->addWidget(title.get());
  centralLayout->addWidget(languageComboBox.get());

  applyStyle();

}