// In StyleSetter.h
#ifndef STYLESETTER_H
#define STYLESETTER_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QScrollArea>
#include <QStackedWidget>

/**
 * @class StyleSetter
 * @brief A class to set styles for various Qt widgets.
 */
class StyleSetter {
public:
  /**
   * @brief Sets the style for a password toggle button.
   * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
   * @param button The QPushButton to style.
   */
  template <typename T>
  void setPasswordToggleButtonStyle(T button);

  /**
   * @brief Sets the style for a checkbox.
   * @tparam T The type of the QCheckBox (shared_ptr or unique_ptr).
   * @param checkBox The QCheckBox to style.
   */
  template<typename T>
  void setCheckBoxStyle(T checkBox);

  /**
   * @brief Sets the style for a combo box.
   * @tparam T The type of the QComboBox (shared_ptr or unique_ptr).
   * @param comboBox The QComboBox to style.
   */
  template<typename T>
  void setComboBoxStyle(T comboBox);

  /**
   * @brief Sets the style for a password field.
   * @tparam T The type of the QLineEdit (shared_ptr or unique_ptr).
   * @param field The QLineEdit to style.
   */
  template <typename T>
  void setPasswordFieldStyle(T field);

  /**
   * @brief Sets the background style for a label.
   * @tparam T The type of the QLabel (shared_ptr or unique_ptr).
   * @param label The QLabel to style.
   */
  template <typename T>
  void setLabelBackgroundStyle(T label);

  /**
   * @brief Sets the style for a widget.
   * @tparam T The type of the QStackedWidget (shared_ptr or unique_ptr).
   * @param widget The QStackedWidget to style.
   */
  template <typename T>
  void setWidgetStyle(T widget);

  /**
   * @brief Sets the style for a button.
   * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
   * @param button The QPushButton to style.
   */
  template <typename T>
  void setButtonStyle(T button);

  /**
   * @brief Sets the style for a line edit.
   * @tparam T The type of the QLineEdit (shared_ptr or unique_ptr).
   * @param lineEdit The QLineEdit to style.
   */
  template<typename T>
  void setLineEditStyle(T lineEdit);

  /**
   * @brief Sets the background style for a widget.
   * @tparam T The type of the QWidget (shared_ptr or unique_ptr).
   * @param widget The QWidget to style.
   */
  template <typename T>
  void setWidgetBackgroundStyle(T widget);

  /**
   * @brief Sets the style for a scroll area.
   * @tparam T The type of the QScrollArea (shared_ptr or unique_ptr).
   * @param scrollArea The QScrollArea to style.
   */
  template <typename T>
  void setScrollAreaStyle(T scrollArea);

  /**
   * @brief Sets the style for a button without a border.
   * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
   * @param button The QPushButton to style.
   */
  template <typename T>
  void setButtonStyleNoBorder(T button);

  /**
   * @brief Sets the style for a label without a border.
   * @tparam T The type of the QLabel (shared_ptr or unique_ptr).
   * @param label The QLabel to style.
   */
  template <typename T>
  void setLabelStyleNoBorder(T label);

  /**
   * @brief Sets the border style for a scroll area.
   * @tparam T The type of the QScrollArea (shared_ptr or unique_ptr).
   * @param scrollArea The QScrollArea to style.
   */
  template <typename T>
  void setScrollAreaBorderStyle(T scrollArea);
};

#include "StyleSetter.tpp"

#endif // STYLESETTER_H