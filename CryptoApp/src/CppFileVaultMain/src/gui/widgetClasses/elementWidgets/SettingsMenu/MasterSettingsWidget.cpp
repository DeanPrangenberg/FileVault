#include "MasterSettingsWidget.h"

MasterSettingsWidget::MasterSettingsWidget(QWidget *parent) : QWidget(parent) {
  applyStyle(this);
}

void MasterSettingsWidget::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  //applyStyle();
}

void MasterSettingsWidget::applyStyle(QWidget *widget) {
  StyleSetter styleSetter;
  styleSetter.setWidgetBackgroundStyle(widget);
}