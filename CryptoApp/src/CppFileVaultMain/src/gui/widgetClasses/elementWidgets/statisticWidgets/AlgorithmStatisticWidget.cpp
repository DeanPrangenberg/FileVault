#include "AlgorithmStatisticWidget.h"

/**
 * @brief Constructs the AlgorithmStatisticWidget object.
 * @param parent The parent widget.
 * @param data The initial data to display in the widget.
 */
AlgorithmStatisticWidget::AlgorithmStatisticWidget(QWidget *parent, const QMap<QString, int> &data)
  : QWidget(parent) {
  setObjectName("AlgorithmStatisticWidget");
  this->setStyleSheet("QWidget#AlgorithmStatisticWidget {"
                       "background-color: rgba(25, 0, 51, 1);"
                       "border: 2px solid rgba(100, 43, 115, 1);"
                       "border-radius: 10px;"
                       "padding: 10px;"
                       "}"
  );
  AlgorithmStatisticWidgetLayout = std::make_unique<QVBoxLayout>(this);
  titelLabel = std::make_unique<QLabel>("Algorithm Statistics", this);
  AlgorithmStatisticWidgetLayout->addWidget(titelLabel.get());
  setLayout(AlgorithmStatisticWidgetLayout.get());
  update(data);
}

/**
 * @brief Updates the widget with new data.
 * @param data The new data to display in the widget.
 */
void AlgorithmStatisticWidget::update(const QMap<QString, int> &data) {
  // Start from 1 to skip the titelLabel
  for (int i = AlgorithmStatisticWidgetLayout->count() - 1; i > 0; --i) {
    QWidget *widget = AlgorithmStatisticWidgetLayout->itemAt(i)->widget();
    if (widget) {
      AlgorithmStatisticWidgetLayout->removeWidget(widget);
      delete widget;
    }
  }
  for (auto it = data.begin(); it != data.end(); ++it) {
    QLabel *label = new QLabel(QString("%1: %2").arg(it.key()).arg(it.value()), this);
    AlgorithmStatisticWidgetLayout->addWidget(label);
  }
}