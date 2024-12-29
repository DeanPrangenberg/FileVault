#ifndef ALGORITHMSTATISTICWIDGET_H
#define ALGORITHMSTATISTICWIDGET_H

#include <QWidget>
#include <memory>
#include <QVBoxLayout>
#include <QLabel>

class AlgorithmStatisticWidget : public QWidget {
Q_OBJECT

public:
  explicit AlgorithmStatisticWidget(QWidget *parent = nullptr, const QMap<QString, int> &data = QMap<QString, int>());
  ~AlgorithmStatisticWidget() override = default;
  void update(const QMap<QString, int> &data);
private:
  std::unique_ptr<QVBoxLayout> AlgorithmStatisticWidgetLayout;
  std::unique_ptr<QLabel> titelLabel;
};

#endif //ALGORITHMSTATISTICWIDGET
