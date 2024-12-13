#include "LanguageSelectionWidget.h"

LanguageSelectionWidget::LanguageSelectionWidget(QWidget *parent) : MasterSettingsWidget(parent) {
  LanguageSelectionWidgetLayout = new QVBoxLayout(this);
  title = new QLabel("Language Selection", this);
  languageComboBox = new QComboBox(this);
  languageComboBox->addItems({"English", "German"});

  LanguageSelectionWidgetLayout->addWidget(title);
  LanguageSelectionWidgetLayout->addWidget(languageComboBox);

  setLayout(LanguageSelectionWidgetLayout);
}

LanguageSelectionWidget::~LanguageSelectionWidget() {
  delete LanguageSelectionWidgetLayout;
}