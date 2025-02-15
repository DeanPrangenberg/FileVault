#include "LoadingWindow.h"

/**
 * @brief Constructs the LoadingWindow object.
 * @param parent The parent widget.
 */
LoadingWindow::LoadingWindow(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Loading");
  setModal(true); // Set the dialog to be modal
  setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint); // Disable the close button

  LoadingWindowLayout = std::make_unique<QVBoxLayout>(this);
  setLayout(LoadingWindowLayout.get());

  // Create and set up the loading label with an animated GIF
  loadingLabel = std::make_unique<QLabel>(this);
  movie = std::make_unique<QMovie>(":/icons/loading.gif");
  loadingLabel->setMovie(movie.get());
  movie->start();
  LoadingWindowLayout->addWidget(loadingLabel.get());

  // Create and set up the console output
  consoleOutput = std::make_unique<QTextEdit>(this);
  consoleOutput->setReadOnly(true);
  LoadingWindowLayout->addWidget(consoleOutput.get());
}

/**
 * @brief Appends text to the console output.
 * @param text The text to append.
 */
void LoadingWindow::appendConsoleOutput(const QString &text) {
  consoleBuffer.push_back(text);

  // Remove lines if the number of lines exceeds maxLines
  if (consoleBuffer.size() > maxLines) {
    consoleBuffer.pop_front();
  }

  updateConsoleOutput();
}

/**
 * @brief Sets the console output to the specified text.
 * @param text The text to set.
 */
void LoadingWindow::setConsoleOutput(const QString &text) {
  consoleOutput->clear();
  consoleOutput->setText(text);
}

/**
 * @brief Updates the console output with the current buffer.
 */
void LoadingWindow::updateConsoleOutput() {
  consoleOutput->clear();
  for (const auto &line : consoleBuffer) {
    consoleOutput->append(line);
  }
}

/**
 * @brief Handles the show event for the dialog.
 * @param event The show event.
 */
void LoadingWindow::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  consoleBuffer.clear(); // Clear the console buffer when the window is shown
  updateConsoleOutput();
}