#ifndef MASTERSETTINGSWIDGET_H
#define MASTERSETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <memory>
#include "../../../styleClass/StyleSetter.h"

/**
 * @class MasterSettingsWidget
 * @brief A base widget for settings with various UI elements.
 *
 * This widget serves as a base class for settings widgets, providing common UI elements
 * such as buttons, combo boxes, check boxes, and line edits.
 */
class MasterSettingsWidget : public QWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs a MasterSettingsWidget.
   *
   * @param parent The parent widget.
   */
  explicit MasterSettingsWidget(QWidget *parent = nullptr);

  /**
   * @brief Destructor for MasterSettingsWidget.
   */
  ~MasterSettingsWidget() override;

protected:
  /// Label for the background of the widget.
  std::shared_ptr<QLabel> backgroundLabel;

  /// Layout for the central part of the widget.
  std::unique_ptr<QVBoxLayout> centralLayout;

  /// Label for the title of the widget.
  std::unique_ptr<QLabel> titleLabel;

  /// Vector of buttons in the widget.
  QVector<std::shared_ptr<QPushButton>> buttons;

  /// Vector of combo boxes in the widget.
  QVector<std::shared_ptr<QComboBox>> comboBoxes;

  /// Vector of check boxes in the widget.
  QVector<std::shared_ptr<QCheckBox>> checkBoxes;

  /// Vector of line edits in the widget.
  QVector<std::shared_ptr<QLineEdit>> lineEdits;

  /// Vector of password fields in the widget.
  QVector<std::shared_ptr<QLineEdit>> passwordFields;

  /// Vector of password toggle buttons in the widget.
  QVector<std::shared_ptr<QPushButton>> passwordToggleButtons;

  /**
   * @brief Applies the style settings to the widget.
   *
   * This function is responsible for applying the predefined style settings
   * to the widget, ensuring a consistent appearance.
   */
  void applyStyle();
};

#endif // MASTERSETTINGSWIDGET_H