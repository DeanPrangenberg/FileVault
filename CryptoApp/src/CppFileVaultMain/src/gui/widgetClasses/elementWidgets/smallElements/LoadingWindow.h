#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMovie>
#include <memory>
#include <deque>

/**
 * @brief The LoadingWindow class provides a dialog window with a loading animation and console output.
 */
class LoadingWindow : public QDialog {
  Q_OBJECT

public:
  /**
   * @brief Constructs the LoadingWindow object.
   * @param parent The parent widget.
   */
  explicit LoadingWindow(QWidget *parent = nullptr);

  /**
   * @brief Destructor for the LoadingWindow object.
   */
  ~LoadingWindow() override = default;

  /**
   * @brief Appends text to the console output.
   * @param text The text to append.
   */
  void appendConsoleOutput(const QString &text);

  /**
   * @brief Sets the console output to the specified text.
   * @param text The text to set.
   */
  void setConsoleOutput(const QString &text);

  int counter = 0; ///< Counter for tracking purposes.

protected:
  /**
   * @brief Handles the show event for the dialog.
   * @param event The show event.
   */
  void showEvent(QShowEvent *event) override;

private:
  std::unique_ptr<QVBoxLayout> LoadingWindowLayout; ///< Layout for the loading window.
  std::unique_ptr<QLabel> loadingLabel; ///< Label for displaying the loading animation.
  std::unique_ptr<QTextEdit> consoleOutput; ///< Text edit for displaying console output.
  std::unique_ptr<QMovie> movie; ///< Movie for the loading animation.
  std::deque<QString> consoleBuffer; ///< Buffer for storing console output lines.
  int maxLines = 100; ///< Maximum number of lines in the console.

  /**
   * @brief Updates the console output with the current buffer.
   */
  void updateConsoleOutput();
};

#endif // LOADINGWINDOW_H