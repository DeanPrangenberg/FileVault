#include "EncryptionScreenWidget.h"

EncryptionScreenWidget::EncryptionScreenWidget(QWidget *parent) : QWidget(parent) {
  EncryptionScreenWidgetLayout = new QVBoxLayout(this);
  setLayout(EncryptionScreenWidgetLayout);
}

EncryptionScreenWidget::~EncryptionScreenWidget() {
  delete EncryptionScreenWidgetLayout;
}
