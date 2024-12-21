#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMovie>
#include <memory>
#include <deque>

class LoadingWindow : public QDialog {
  Q_OBJECT

public:
  explicit LoadingWindow(QWidget *parent = nullptr);
  ~LoadingWindow() override = default;
  void appendConsoleOutput(const QString &text);
  void setConsoleOutput(const QString &text);
  int counter = 0;

protected:
  void showEvent(QShowEvent *event) override;

private:
  std::unique_ptr<QVBoxLayout> LoadingWindowLayout;
  std::unique_ptr<QLabel> loadingLabel;
  std::unique_ptr<QTextEdit> consoleOutput;
  std::unique_ptr<QMovie> movie;
  std::deque<QString> consoleBuffer;
  int maxLines = 100; // Maximum number of lines in the console
  void updateConsoleOutput();
};

#endif // LOADINGWINDOW_H