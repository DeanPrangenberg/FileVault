#ifndef CAKEDIAGRAM_H
#define CAKEDIAGRAM_H

#include <QWidget>
#include <memory>
#include <QVBoxLayout>
#include <QMap>

/**
 * @brief The CakeDiagram class provides a widget for displaying a pie chart diagram.
 */
class CakeDiagram : public QWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs the CakeDiagram object.
   * @param parent The parent widget.
   */
  explicit CakeDiagram(QWidget *parent = nullptr);

  /**
   * @brief Destructor for the CakeDiagram object.
   */
  ~CakeDiagram() override = default;

  /**
   * @brief Resets the diagram by clearing all values and updating the display.
   */
  void reset();

  /**
   * @brief Adds a value to the diagram.
   * @param label The label for the value.
   * @param value The value to add.
   */
  void addValue(const QString &label, double value);

  QMap<QString, double> values; ///< Map storing the values and their labels.

protected:
  /**
   * @brief Paints the diagram.
   * @param event The paint event.
   */
  void paintEvent(QPaintEvent *event) override;

private:
  std::unique_ptr<QVBoxLayout> CakeDiagramLayout; ///< Layout for the diagram widget.
  double totalAmount; ///< Total amount of all values in the diagram.
};

#endif //CAKEDIAGRAM_H