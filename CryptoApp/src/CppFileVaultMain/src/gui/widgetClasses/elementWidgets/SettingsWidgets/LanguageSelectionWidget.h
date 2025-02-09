#ifndef LANGUAGESELECTIONWIDGET_H
#define LANGUAGESELECTIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "MasterSettingsWidget.h"

/**
 * @class LanguageSelectionWidget
 * @brief A widget for selecting the language.
 *
 * This widget provides a combo box for selecting the language from a list of available languages.
 */
class LanguageSelectionWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs a LanguageSelectionWidget.
   *
   * @param parent The parent widget.
   */
  explicit LanguageSelectionWidget(QWidget *parent = nullptr);

  /**
   * @brief Destructor for LanguageSelectionWidget.
   */
  ~LanguageSelectionWidget() override = default;

  /// Combo box for selecting the language.
  std::shared_ptr<QComboBox> languageComboBox;
};

#endif // LANGUAGESELECTIONWIDGET_H