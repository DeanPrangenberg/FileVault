//
// Created by prang on 25.09.2024.
//

#ifndef RANSOMWARE_MAINWINDOW_H
#define RANSOMWARE_MAINWINDOW_H

#include <QWidget>
#include <QTextEdit>

class RansomWindow : public QWidget {
Q_OBJECT

public:
  RansomWindow(QWidget *parent = nullptr);
  virtual ~RansomWindow();
private slots:
  void payRansom();
  void startDecryption();

private:
  void loadMainWidget();

  bool ransomPayed;

  QTextEdit *keyInputLineEdit;

  QTextEdit *ivInputLineEdit;
};

#endif //RANSOMWARE_MAINWINDOW_H
