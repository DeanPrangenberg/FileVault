#ifndef ALGORITHMSTATISTICWIDGET_H
#define ALGORITHMSTATISTICWIDGET_H

#include <QWidget>
#include <memory>
#include <QVBoxLayout>
#include <QLabel>

/**
 * @brief The AlgorithmStatisticWidget class provides a widget for displaying algorithm statistics.
 */
class AlgorithmStatisticWidget : public QWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs the AlgorithmStatisticWidget object.
   * @param parent The parent widget.
   * @param data The initial data to display in the widget.
   */
  explicit AlgorithmStatisticWidget(QWidget *parent = nullptr, const QMap<QString, int> &data = QMap<QString, int>());

  /**
   * @brief Destructor for the AlgorithmStatisticWidget object.
   */
  ~AlgorithmStatisticWidget() override = default;

  /**
   * @brief Updates the widget with new data.
   * @param data The new data to display in the widget.
   */
  void update(const QMap<QString, int> &data);

private:
  std::unique_ptr<QVBoxLayout> AlgorithmStatisticWidgetLayout; ///< Layout for the widget.
  std::unique_ptr<QLabel> titelLabel; ///< Label for the title of the widget.
};

#endif //ALGORITHMSTATISTICWIDGET_H