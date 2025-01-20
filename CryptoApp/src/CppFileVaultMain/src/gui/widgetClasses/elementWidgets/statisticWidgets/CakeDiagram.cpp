#include "CakeDiagram.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <QFontMetrics>

/**
 * @brief Constructs the CakeDiagram object.
 * @param parent The parent widget.
 */
CakeDiagram::CakeDiagram(QWidget *parent) : QWidget(parent), totalAmount(0) {
  CakeDiagramLayout = std::make_unique<QVBoxLayout>(this);
  setLayout(CakeDiagramLayout.get());
  setMinimumSize(200, 200); // Set a minimum size for the widget
}

/**
 * @brief Resets the diagram by clearing all values and updating the display.
 */
void CakeDiagram::reset() {
  values.clear();
  totalAmount = 0;
  update();
}

/**
 * @brief Adds a value to the diagram.
 * @param label The label for the value.
 * @param value The value to add.
 */
void CakeDiagram::addValue(const QString &label, double value) {
  values[label] = value;
  totalAmount += value;
  update();
}

/**
 * @brief Paints the diagram.
 * @param event The paint event.
 */
void CakeDiagram::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  int margin = 20;
  int legendWidth = 150;
  int side = qMin(width() - legendWidth - margin * 3, height() - margin * 2);
  QRectF rect(margin, margin, side, side);

  double startAngle = 0.0;
  QColor borderColor(100, 43, 115);

  const QVector<QColor> backgroundColors = {
      QColor(150, 100, 200, 255),  // Light purple
      QColor(50, 0, 100, 255),     // Dark violet
      QColor(200, 150, 250, 255),  // Light lavender
      QColor(100, 50, 150, 255),   // Medium purple
      QColor(120, 80, 180, 255),   // Soft purple
      QColor(80, 30, 120, 255),    // Deep violet
      QColor(170, 120, 230, 255),  // Delicate violet
      QColor(130, 90, 210, 255)    // Medium purple
  };

  const QVector<QColor> textColors = {
      QColor(255, 255, 255, 255),  // White
      QColor(200, 150, 250, 255),  // Light lavender
      QColor(50, 0, 100, 255),     // Dark violet
      QColor(255, 255, 255, 255),  // White
      QColor(50, 0, 100, 255),     // Dark violet
      QColor(200, 150, 250, 255),  // Light lavender
      QColor(50, 0, 100, 255),     // Dark violet
      QColor(255, 255, 255, 255)   // White
  };

  int colorIndex = 0;

  // Draw the pie chart
  int sectionCount = 0;
  for (auto it = values.begin(); it != values.end() && sectionCount < 8; ++it, ++sectionCount) {
    double percentage = it.value() / totalAmount;
    double spanAngle = 360.0 * percentage;

    painter.setBrush(backgroundColors[colorIndex % 8]);
    painter.setPen(borderColor);
    painter.drawPie(rect, startAngle * 16, spanAngle * 16);

    // Draw the label in the center of each section
    double angle = startAngle + spanAngle / 2;
    double x = rect.center().x() + (rect.width() / 4) * qCos(qDegreesToRadians(angle));
    double y = rect.center().y() - (rect.height() / 4) * qSin(qDegreesToRadians(angle));
    QString label = it.key();

    painter.setPen(textColors[colorIndex % 8]);
    painter.drawText(QPointF(x, y), label);

    startAngle += spanAngle;
    colorIndex++;
  }

  // Draw the legend
  int legendY = margin;
  QFont font = painter.font();
  painter.setFont(font);
  QFontMetrics fm(font);

  sectionCount = 0;
  for (auto it = values.begin(); it != values.end() && sectionCount < 8; ++it, ++sectionCount) {
    double percentage = it.value() / totalAmount;

    QRect colorRect(width() - legendWidth, legendY, 20, 20);
    painter.setPen(Qt::white);
    painter.fillRect(colorRect, backgroundColors[colorIndex % 8]);
    painter.drawRect(colorRect);

    painter.setPen(Qt::white); // Set the text color to white
    QString legendText = QString("%1: %2% (%3)").arg(it.key()).arg(percentage * 100, 0, 'f', 1).arg(it.value());
    QRect textRect(width() - legendWidth + 25, legendY, legendWidth - 25, 20);
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, legendText);

    legendY += 25;
    ++colorIndex;
  }
}