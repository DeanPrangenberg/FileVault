#include "StyleSetter.h"

void StyleSetter::setButtonStyle(QPushButton *button) {
  int buttonHeight = button->height();
  int borderRadius = buttonHeight / 40;

  const int maxBorderRadius = 20;
  const int minBorderRadius = 5;

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
                                "padding: 10px 20px;"
                                "font-size: 16px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgba(15, 0, 31, 1);"
                                "border-color: rgba(150, 93, 165, 1);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgba(10, 0, 21, 1);"
                                "border-color: rgba(200, 143, 215, 1);"
                                "}").arg(borderRadius);

  qDebug() << "Setting button style:" << buttonStyle;
  button->setStyleSheet(buttonStyle);
}

void StyleSetter::setWidgetStyle(QWidget *widget) {
  QString widgetStyle = QString("QWidget {"
                                "background-color: rgba(25, 0, 51, 1);"
                                "color: white;"
                                "border: 2px solid rgba(100, 43, 115, 1);"
                                "border-radius: 10px;"
                                "padding: 10px 20px;"
                                "font-size: 16px;"
                                "}");

  qDebug() << "Setting widget style:" << widgetStyle;
  widget->setStyleSheet(widgetStyle);
}

void StyleSetter::setWidgetBackgroundStyle(QWidget *widget) {
  if (widget->objectName().isEmpty()) {
    widget->setObjectName("defaultWidget");
    qDebug() << "Object name was empty, set to defaultWidget";
  } else {
    qDebug() << "Object name is:" << widget->objectName();
  }

  QString widgetStyle = QString("MasterSettingsWidget#%1 {"
                                "background-color: rgba(25, 0, 51, 1);"
                                "border: 2px solid rgba(100, 43, 115, 1);"
                                "border-radius: 10px;"
                                "padding: 10px;"
                                "}").arg(widget->objectName());

  qDebug() << "Setting widget background style:" << widgetStyle;
  widget->setStyleSheet(widgetStyle);
}