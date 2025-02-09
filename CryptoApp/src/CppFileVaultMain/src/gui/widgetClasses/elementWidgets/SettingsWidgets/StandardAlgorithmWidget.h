#ifndef STANDARDALGORITHMWIDGET_H
#define STANDARDALGORITHMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "MasterSettingsWidget.h"

/**
 * @class StandardAlgorithmWidget
 * @brief A widget for selecting a standard algorithm.
 *
 * This widget allows the user to select a standard algorithm from a combo box.
 */
class StandardAlgorithmWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs a StandardAlgorithmWidget.
   *
   * @param parent The parent widget.
   */
  explicit StandardAlgorithmWidget(QWidget *parent = nullptr);

  /// Combo box for selecting the algorithm.
  std::shared_ptr<QComboBox> algorithmComboBox;
};

#endif // STANDARDALGORITHMWIDGET_H