// StyleSetter.h
#ifndef STYLESETTER_H
#define STYLESETTER_H

#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include "widgetClasses/elementWidgets/SettingsMenu/MasterSettingsWidget.h"

class StyleSetter {
public:
  void setButtonStyle(QPushButton *button);
  void setWidgetStyle(QWidget *widget);
  void setWidgetBackgroundStyle(QWidget *widget);
};

#endif // STYLESETTER_H