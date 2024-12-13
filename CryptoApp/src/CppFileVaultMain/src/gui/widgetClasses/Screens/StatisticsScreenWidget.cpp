#include "StatisticsScreenWidget.h"

StatisticsScreenWidget::StatisticsScreenWidget(QWidget *parent) : QWidget(parent) {
  StatisticsScreenWidgetLayout = new QVBoxLayout(this);
  setLayout(StatisticsScreenWidgetLayout);
}

StatisticsScreenWidget::~StatisticsScreenWidget() {
  delete StatisticsScreenWidgetLayout;
}
