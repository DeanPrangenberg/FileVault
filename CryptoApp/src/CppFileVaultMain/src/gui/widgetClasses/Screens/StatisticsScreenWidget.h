#ifndef STATISTICSSCREENWIDGET_H
#define STATISTICSSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class StatisticsScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit StatisticsScreenWidget(QWidget *parent = nullptr);
  ~StatisticsScreenWidget() override;
private:
  QVBoxLayout *StatisticsScreenWidgetLayout;

};

#endif //STATISTICSSCREENWIDGET
