#include "DecryptionScreenWidget.h"

DecryptionScreenWidget::DecryptionScreenWidget(QWidget *parent) : QWidget(parent) {
  DecryptionScreenWidgetLayout = new QVBoxLayout(this);
  setLayout(DecryptionScreenWidgetLayout);
}

DecryptionScreenWidget::~DecryptionScreenWidget() {
  delete DecryptionScreenWidgetLayout;
}
