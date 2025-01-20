#ifndef STYLESETTER_TPP
#define STYLESETTER_TPP

#include "StyleSetter.h"

/**
 * @brief Sets the style for a password toggle button.
 * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
 * @param button The QPushButton to style.
 */
template<typename T>
void StyleSetter::setPasswordToggleButtonStyle(T button) {
  QString buttonStyle = QString("QPushButton {"
                                "border: none;"
                                "background: transparent;"
                                "}"
  );
  button->setStyleSheet(buttonStyle);
}

/**
 * @brief Sets the style for a combo box.
 * @tparam T The type of the QComboBox (shared_ptr or unique_ptr).
 * @param comboBox The QComboBox to style.
 */
template<typename T>
void StyleSetter::setComboBoxStyle(T comboBox) {
  QString comboBoxStyle = QString("QComboBox {"
                                  "background-color: rgba(25, 0, 51, 1);"
                                  "color: white;"
                                  "font-size: 14px;"
                                  "border: 2px solid rgba(100, 43, 115, 1);"
                                  "border-radius: 5px;"
                                  "padding: 5px 5px 5px 5px;"
                                  "min-width: 6em;"
                                  "}"
                                  "QComboBox::drop-down {"
                                  "subcontrol-origin: padding;"
                                  "subcontrol-position: top right;"
                                  "width: 15px;"
                                  "border: none;"
                                  "margin-right: 15px;"
                                  "}"
                                  "QComboBox::down-arrow {"
                                  "width: 10px;"
                                  "height: 10px;"
                                  "image: url(:/icons/dropdown.png);"
                                  "}"
                                  "QComboBox:hover {"
                                  "background-color: rgba(15, 0, 31, 1);"
                                  "border-color: rgba(150, 93, 165, 1);"
                                  "}"
                                  "QComboBox QAbstractItemView {"
                                  "background-color: rgba(25, 0, 51, 1);"
                                  "color: white;"
                                  "selection-background-color: rgba(35, 10, 71, 1);"
                                  "border: 2px solid rgba(100, 43, 115, 1);"
                                  "border-radius: 5px;"
                                  "}"
                                  "QComboBox QAbstractItemView::item:hover {"
                                  "background-color: rgba(35, 10, 71, 1);"
                                  "}"
                                  "QComboBox QAbstractItemView::item:selected {"
                                  "background-color: rgba(45, 20, 91, 1);"
                                  "}"
  );
  comboBox->setStyleSheet(comboBoxStyle);
  comboBox->setAttribute(Qt::WA_NoMouseReplay, true);  // Suppress default Windows selection frame
}

/**
 * @brief Sets the style for a checkbox.
 * @tparam T The type of the QCheckBox (shared_ptr or unique_ptr).
 * @param checkBox The QCheckBox to style.
 */
template<typename T>
void StyleSetter::setCheckBoxStyle(T checkBox) {
  QString checkBoxStyle = QString("QCheckBox {"
                                  "background-color: rgba(25, 0, 51, 1);"
                                  "spacing: 5px;"
                                  "color: white;"
                                  "padding: 5px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QCheckBox::indicator {"
                                  "width: 20px;"
                                  "height: 20px;"
                                  "border: 2px solid rgba(100, 43, 115, 1);"
                                  "border-radius: 5px;"
                                  "}"
                                  "QCheckBox::indicator:checked {"
                                  "image: url(:/icons/checkmark.png);"
                                  "}"
                                  "QCheckBox::indicator:hover {"
                                  "background-color: rgba(15, 0, 31, 1);"
                                  "border: 2px solid rgba(150, 93, 165, 1);"
                                  "border-radius: 5px;"
                                  "}"
  );
  checkBox->setStyleSheet(checkBoxStyle);
}

/**
 * @brief Sets the style for a password field.
 * @tparam T The type of the QLineEdit (shared_ptr or unique_ptr).
 * @param field The QLineEdit to style.
 */
template<typename T>
void StyleSetter::setPasswordFieldStyle(T field) {
  QString fieldStyle = QString("QLineEdit {"
                               "background: transparent;"
                               "color: white;"
                               "border-radius: 10px;"
                               "padding: 5px 10px;"
                               "font-size: 14px;"
                               "}"
                               "QLineEdit:focus {"
                               "border-color: rgba(150, 93, 165, 1);"
                               "}"
  );
  field->setStyleSheet(fieldStyle);
  field->setEchoMode(QLineEdit::Password);
}

/**
 * @brief Sets the background style for a label.
 * @tparam T The type of the QLabel (shared_ptr or unique_ptr).
 * @param label The QLabel to style.
 */
template<typename T>
void StyleSetter::setLabelBackgroundStyle(T label) {
  QString labelStyle = QString("QLabel {"
                               "background-color: rgba(25, 0, 51, 1);"
                               "color: white;"
                               "border: 2px solid rgba(100, 43, 115, 1);"
                               "border-radius: 10px;"
                               "padding: 10px 20px;"
                               "font-size: 14px;"
                               "}"
  );
  label->setStyleSheet(labelStyle);
}

/**
 * @brief Sets the style for a widget.
 * @tparam T The type of the QStackedWidget (shared_ptr or unique_ptr).
 * @param widget The QStackedWidget to style.
 */
template<typename T>
void StyleSetter::setWidgetStyle(T widget) {
  if (widget->objectName().isEmpty()) {
    widget->setObjectName("defaultWidget");
  }
  QString widgetStyle = QString("QWidget#%1 {"
                                "background-color: rgba(25, 0, 51, 1);"
                                "color: white;"
                                "border: 2px solid rgba(100, 43, 115, 1);"
                                "border-radius: 10px;"
                                "padding: 10px 20px;"
                                "font-size: 14px;"
                                "}"
  ).arg(widget->objectName());
  widget->setStyleSheet(widgetStyle);
}

/**
 * @brief Sets the style for a button.
 * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
 * @param button The QPushButton to style.
 */
template<typename T>
void StyleSetter::setButtonStyle(T button) {
  int buttonHeight = button->height();
  int borderRadius = buttonHeight / 40;

  int maxBorderRadius = 20;
  int minBorderRadius = 5;

  if (borderRadius > maxBorderRadius) {
    borderRadius = maxBorderRadius;
  } else if (borderRadius < minBorderRadius) {
    borderRadius = minBorderRadius;
  }

  QString buttonStyle = QString("QPushButton {"
                                "background-color: rgba(25, 0, 51, 1);"
                                "color: white;"
                                "border: 2px solid rgba(100, 43, 115, 1);"
                                "border-radius: %1px;"
                                "font-size: %2px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgba(15, 0, 31, 1);"
                                "border-color: rgba(150, 93, 165, 1);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgba(10, 0, 21, 1);"
                                "border-color: rgba(200, 143, 215, 1);"
                                "}"
  ).arg(borderRadius).arg(buttonHeight >= 32 ? 16 : buttonHeight / 2.5);
  button->setStyleSheet(buttonStyle);
}

/**
 * @brief Sets the style for a line edit.
 * @tparam T The type of the QLineEdit (shared_ptr or unique_ptr).
 * @param lineEdit The QLineEdit to style.
 */
template<typename T>
void StyleSetter::setLineEditStyle(T lineEdit) {
  QString lineEditStyle = QString("QLineEdit {"
                                  "background-color: rgba(25, 0, 51, 1);"
                                  "color: white;"
                                  "border: 2px solid rgba(100, 43, 115, 1);"
                                  "border-radius: 10px;"
                                  "padding: 5px 10px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QLineEdit:focus {"
                                  "border-color: rgba(150, 93, 165, 1);"
                                  "}"
  );
  lineEdit->setStyleSheet(lineEditStyle);
}

/**
 * @brief Sets the background style for a widget.
 * @tparam T The type of the QWidget (shared_ptr or unique_ptr).
 * @param widget The QWidget to style.
 */
template<typename T>
void StyleSetter::setWidgetBackgroundStyle(T widget) {
  QString widgetStyle = QString("QWidget#%1 {"
                                "background-color: rgba(25, 0, 51, 1);"
                                "border: 2px solid rgba(100, 43, 115, 1);"
                                "border-radius: 10px;"
                                "padding: 10px;"
                                "}"
  ).arg("MasterSettingsWidget");
  widget->setStyleSheet(widgetStyle);
}

/**
 * @brief Sets the style for a scroll area.
 * @tparam T The type of the QScrollArea (shared_ptr or unique_ptr).
 * @param scrollArea The QScrollArea to style.
 */
template<typename T>
void StyleSetter::setScrollAreaStyle(T scrollArea) {
  QString scrollAreaStyle = QString("QScrollArea {"
                                    "background: transparent;"
                                    "border: none;"
                                    "}"
                                    "QScrollBar:vertical {"
                                    "background: transparent;"
                                    "width: 13px;"
                                    "margin: 0px 3px 0px 3px;"
                                    "border-radius: 20px;"  // Rounded corners
                                    "}"
                                    "QScrollBar::handle:vertical {"
                                    "background: rgba(100, 43, 115, 1);"
                                    "min-height: 20px;"
                                    "border-radius: 10px;"  // Rounded corners
                                    "}"
                                    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                                    "background: none;"
                                    "}"
                                    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                    "background: none;"
                                    "}"
                                    "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                                    "width: 0;"
                                    "height: 0;"
                                    "border-left: 7px solid transparent;"
                                    "border-right: 7px solid transparent;"
                                    "}"
  );
  scrollArea->setStyleSheet(scrollAreaStyle);
}

/**
 * @brief Sets the style for a button without a border.
 * @tparam T The type of the QPushButton (shared_ptr or unique_ptr).
 * @param button The QPushButton to style.
 */
template<typename T>
void StyleSetter::setButtonStyleNoBorder(T button) {
  int buttonHeight = button->height();
  int borderRadius = buttonHeight / 40;

  int maxBorderRadius = 20;
  int minBorderRadius = 5;

  if (borderRadius > maxBorderRadius) {
    borderRadius = maxBorderRadius;
  } else if (borderRadius < minBorderRadius) {
    borderRadius = minBorderRadius;
  }

  QString buttonStyle = QString("QPushButton {"
                                "background-color: rgba(25, 0, 51, 1);"
                                "color: white;"
                                "border-radius: %1px;"
                                "font-size: %2px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgba(15, 0, 31, 1);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgba(10, 0, 21, 1);"
                                "}"
  ).arg(borderRadius).arg(buttonHeight >= 32 ? 16 : buttonHeight / 2.5);
  button->setStyleSheet(buttonStyle);
}

/**
 * @brief Sets the style for a label without a border.
 * @tparam T The type of the QLabel (shared_ptr or unique_ptr).
 * @param label The QLabel to style.
 */
template<typename T>
void StyleSetter::setLabelStyleNoBorder(T label) {
  QString labelStyle = QString("QLabel {"
                               "background-color: rgba(25, 0, 51, 1);"
                               "color: white;"
                               "border-radius: 10px;"
                               "padding: 10px 20px;"
                               "font-size: 14px;"
                               "}"
  );
  label->setStyleSheet(labelStyle);
}

/**
 * @brief Sets the border style for a scroll area.
 * @tparam T The type of the QScrollArea (shared_ptr or unique_ptr).
 * @param scrollArea The QScrollArea to style.
 */
template<typename T>
void StyleSetter::setScrollAreaBorderStyle(T scrollArea) {
  QString scrollAreaStyle = QString("QScrollArea {"
                                    "background-color: rgba(25, 0, 51, 1);"
                                    "border: 2px solid rgba(100, 43, 115, 1);"
                                    "border-radius: 10px;"
                                    "padding: 10px;"
                                    "}"
                                    "QScrollBar:vertical {"
                                    "background: transparent;"
                                    "width: 13px;"
                                    "margin: 0px 3px 0px 3px;"
                                    "border-radius: 20px;"
                                    "}"
                                    "QScrollBar::handle:vertical {"
                                    "background: rgba(100, 43, 115, 1);"
                                    "min-height: 20px;"
                                    "border-radius: 10px;"
                                    "}"
                                    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                                    "background: none;"
                                    "}"
                                    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                    "background: none;"
                                    "}"
                                    "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                                    "width: 0;"
                                    "height: 0;"
                                    "border-left: 7px solid transparent;"
                                    "border-right: 7px solid transparent;"
                                    "}"
  );
  scrollArea->setStyleSheet(scrollAreaStyle);
}

#endif // STYLESETTER_TPP