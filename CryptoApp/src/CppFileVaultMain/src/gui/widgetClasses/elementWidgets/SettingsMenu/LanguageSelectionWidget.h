#ifndef LANGUAGESELECTIONWIDGET_H
#define LANGUAGESELECTIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "MasterSettingsWidget.h"

class LanguageSelectionWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  explicit LanguageSelectionWidget(QWidget *parent = nullptr);
  ~LanguageSelectionWidget() override;

  QComboBox *languageComboBox;

private:
  QVBoxLayout *LanguageSelectionWidgetLayout;
  QLabel *title;
};

#endif // LANGUAGESELECTIONWIDGET_H