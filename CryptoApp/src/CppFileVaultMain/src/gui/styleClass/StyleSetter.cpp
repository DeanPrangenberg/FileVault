#include "StyleSetter.h"

/**
 * @brief Sets the style for a passwordHash toggle button.
 * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
 * @param button The QPushButton to style.
 */
template void StyleSetter::setPasswordToggleButtonStyle(std::shared_ptr<QPushButton> button);
template void StyleSetter::setPasswordToggleButtonStyle(std::unique_ptr<QPushButton> button);

/**
 * @brief Sets the style for a checkbox.
 * @tparam T The type of the QCheckBox (shared_ptr or unique_ptr).
 * @param checkBox The QCheckBox to style.
 */
template void StyleSetter::setCheckBoxStyle(std::shared_ptr<QCheckBox> checkBox);
template void StyleSetter::setCheckBoxStyle(std::unique_ptr<QCheckBox> checkBox);

/**
 * @brief Sets the style for a combo box.
 * @tparam T The type of the QComboBox (shared_ptr or unique_ptr).
 * @param comboBox The QComboBox to style.
 */
template void StyleSetter::setComboBoxStyle(std::shared_ptr<QComboBox> comboBox);
template void StyleSetter::setComboBoxStyle(std::unique_ptr<QComboBox> comboBox);

/**
 * @brief Sets the style for a passwordHash field.
 * @tparam T The type of the QLineEdit (shared_ptr or unique_ptr).
 * @param passwordField The QLineEdit to style.
 */
template void StyleSetter::setPasswordFieldStyle(std::shared_ptr<QLineEdit> passwordField);
template void StyleSetter::setPasswordFieldStyle(std::unique_ptr<QLineEdit> passwordField);

/**
 * @brief Sets the background style for a label.
 * @tparam T The type of the QLabel (shared_ptr or unique_ptr).
 * @param label The QLabel to style.
 */
template void StyleSetter::setLabelBackgroundStyle(std::shared_ptr<QLabel> label);
template void StyleSetter::setLabelBackgroundStyle(std::unique_ptr<QLabel> label);

/**
 * @brief Sets the style for a widget.
 * @tparam T The type of the QStackedWidget (shared_ptr or unique_ptr).
 * @param widget The QStackedWidget to style.
 */
template void StyleSetter::setWidgetStyle(std::shared_ptr<QStackedWidget> widget);
template void StyleSetter::setWidgetStyle(std::unique_ptr<QStackedWidget> widget);

/**
 * @brief Sets the style for a button.
 * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
 * @param button The QPushButton to style.
 */
template void StyleSetter::setButtonStyle(std::shared_ptr<QPushButton> button);
template void StyleSetter::setButtonStyle(std::unique_ptr<QPushButton> button);

/**
 * @brief Sets the style for a line edit.
 * @tparam T The type of the QLineEdit (shared_ptr or unique_ptr).
 * @param lineEdit The QLineEdit to style.
 */
template void StyleSetter::setLineEditStyle(std::shared_ptr<QLineEdit> lineEdit);
template void StyleSetter::setLineEditStyle(std::unique_ptr<QLineEdit> lineEdit);

/**
 * @brief Sets the background style for a widget.
 * @tparam T The type of the QWidget (shared_ptr or unique_ptr).
 * @param widget The QWidget to style.
 */
template void StyleSetter::setWidgetBackgroundStyle(std::shared_ptr<QWidget> widget);
template void StyleSetter::setWidgetBackgroundStyle(std::unique_ptr<QWidget> widget);

/**
 * @brief Sets the style for a scroll area.
 * @tparam T The type of the QScrollArea (shared_ptr or unique_ptr).
 * @param scrollArea The QScrollArea to style.
 */
template void StyleSetter::setScrollAreaStyle(std::shared_ptr<QScrollArea> scrollArea);
template void StyleSetter::setScrollAreaStyle(std::unique_ptr<QScrollArea> scrollArea);

/**
 * @brief Sets the style for a button without a border.
 * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
 * @param button The QPushButton to style.
 */
template void StyleSetter::setButtonStyleNoBorder(std::shared_ptr<QPushButton> button);
template void StyleSetter::setButtonStyleNoBorder(std::unique_ptr<QPushButton> button);

/**
 * @brief Sets the style for a label without a border.
 * @tparam T The type of the QLabel (shared_ptr or unique_ptr).
 * @param label The QLabel to style.
 */
template void StyleSetter::setLabelStyleNoBorder(std::shared_ptr<QLabel> label);
template void StyleSetter::setLabelStyleNoBorder(std::unique_ptr<QLabel> label);

/**
 * @brief Sets the border style for a scroll area.
 * @tparam T The type of the QScrollArea (shared_ptr or unique_ptr).
 * @param scrollArea The QScrollArea to style.
 */
template void StyleSetter::setScrollAreaBorderStyle(std::shared_ptr<QScrollArea> scrollArea);
template void StyleSetter::setScrollAreaBorderStyle(std::unique_ptr<QScrollArea> scrollArea);