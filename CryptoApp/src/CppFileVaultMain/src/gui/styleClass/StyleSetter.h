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

class StyleSetter {
public:
  template <typename T>
  void setPasswordToggleButtonStyle(T button);

  template<typename T>
  void setCheckBoxStyle(T checkBox);

  template<typename T>
  void setComboBoxStyle(T comboBox);

  template <typename T>
  void setPasswordFieldStyle(T field);

  template <typename T>
  void setLabelBackgroundStyle(T label);

  template <typename T>
  void setWidgetStyle(T widget);

  template <typename T>
  void setButtonStyle(T button);

  template<typename T>
  void setLineEditStyle(T lineEdit);

  template <typename T>
  void setWidgetBackgroundStyle(T widget);

  template <typename T>
  void setScrollAreaStyle(T scrollArea);

  template <typename T>
  void setButtonStyleNoBorder(T button);

  template <typename T>
  void setLabelStyleNoBorder(T label);
};

#include "StyleSetter.tpp"

#endif // STYLESETTER_H