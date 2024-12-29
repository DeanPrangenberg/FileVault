#ifndef CAKEDIAGRAM_H
#define CAKEDIAGRAM_H

#include <QWidget>
#include <memory>
#include <QVBoxLayout>
#include <QMap>

class CakeDiagram : public QWidget {
Q_OBJECT

public:
  explicit CakeDiagram(QWidget *parent = nullptr);
  ~CakeDiagram() override = default;

  void reset();
  void addValue(const QString &label, double value);

  QMap<QString, double> values;
protected:
  void paintEvent(QPaintEvent *event) override;

private:
  std::unique_ptr<QVBoxLayout> CakeDiagramLayout;
  double totalAmount;
};

#endif //CAKEDIAGRAM_H