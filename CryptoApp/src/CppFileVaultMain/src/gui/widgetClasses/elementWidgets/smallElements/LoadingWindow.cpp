#include "LoadingWindow.h"

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

void LoadingWindow::appendConsoleOutput(const QString &text) {
  consoleOutput->append(text);

  // Remove lines if the number of lines exceeds maxLines
  while (consoleOutput->document()->blockCount() > maxLines) {
    QTextCursor cursor = consoleOutput->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.select(QTextCursor::BlockUnderCursor);
    cursor.removeSelectedText();
    cursor.deleteChar();
  }
}