#ifndef STYLESETTER_TPP
#define STYLESETTER_TPP

#include "StyleSetter.h"

template <typename T>
void StyleSetter::setPasswordToggleButtonStyle(T button) {
  QString buttonStyle = QString("QPushButton {"
                                "border: none;"
                                "background: transparent;"
                                "}");
  button->setStyleSheet(buttonStyle);
}

template <typename T>
void StyleSetter::setCheckBoxStyle(T checkBox) {
  QString checkBoxStyle = QString("QCheckBox {"
                                  "color: white;"
                                  "}"
                                  "QCheckBox::indicator {"
                                  "width: 20px;"
                                  "height: 20px;"
                                  "}"
                                  "QCheckBox::indicator:unchecked {"
                                  "image: url(:/images/unchecked.png);"
                                  "}"
                                  "QCheckBox::indicator:checked {"
                                  "image: url(:/images/checked.png);"
                                  "}");
  checkBox->setStyleSheet(checkBoxStyle);
}

template<typename T>
void StyleSetter::setComboBoxStyle(T comboBox) {
  QString comboBoxStyle = QString("QComboBox {"
                                  "background: transparent;"
                                  "color: white;"
                                  "border: 2px solid rgba(100, 43, 115, 1);"
                                  "border-radius: 10px;"
                                  "padding: 5px 10px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QComboBox::drop-down {"
                                  "border: none;"
                                  "}"
                                  "QComboBox::down-arrow {"
                                  "image: url(:/images/down-arrow.png);"
                                  "}");
  comboBox->setStyleSheet(comboBoxStyle);
}

template <typename T>
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
                               "}");
  field->setStyleSheet(fieldStyle);
  field->setEchoMode(QLineEdit::Password);
}

template <typename T>
void StyleSetter::setLabelBackgroundStyle(T label) {
  QString labelStyle = QString("QLabel {"
                               "background-color: rgba(25, 0, 51, 1);"
                               "color: white;"
                               "border: 2px solid rgba(100, 43, 115, 1);"
                               "border-radius: 10px;"
                               "padding: 10px 20px;"
                               "font-size: 14px;"
                               "}");
  label->setStyleSheet(labelStyle);
}

template <typename T>
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
                                "}").arg(widget->objectName());
  widget->setStyleSheet(widgetStyle);
}

template <typename T>
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
                                "}").arg(borderRadius).arg(buttonHeight >= 32 ? 16 : buttonHeight / 2.5);
  button->setStyleSheet(buttonStyle);
}

template <typename T>
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
                                  "}");
  lineEdit->setStyleSheet(lineEditStyle);
}

template <typename T>
void StyleSetter::setWidgetBackgroundStyle(T widget) {
  QString widgetStyle = QString("QWidget#%1 {"
                                "background-color: rgba(25, 0, 51, 1);"
                                "border: 2px solid rgba(100, 43, 115, 1);"
                                "border-radius: 10px;"
                                "padding: 10px;"
                                "}").arg("MasterSettingsWidget");
  widget->setStyleSheet(widgetStyle);
}

#endif // STYLESETTER_TPP