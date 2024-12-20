#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMovie>
#include <memory>

class LoadingWindow : public QDialog {
  Q_OBJECT

public:
  explicit LoadingWindow(QWidget *parent = nullptr);
  ~LoadingWindow() override = default;
  void appendConsoleOutput(const QString &text);

private:
  std::unique_ptr<QVBoxLayout> LoadingWindowLayout;
  std::unique_ptr<QLabel> loadingLabel;
  std::unique_ptr<QTextEdit> consoleOutput;
  std::unique_ptr<QMovie> movie;
  int maxLines = 100; // Maximum number of lines in the console
};

#endif // LOADINGWINDOW_H