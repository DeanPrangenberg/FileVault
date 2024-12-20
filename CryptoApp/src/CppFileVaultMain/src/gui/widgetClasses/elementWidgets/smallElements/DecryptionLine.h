#ifndef DECRYPTIONLINE_H
#define DECRYPTIONLINE_H

#include <QWidget>
#include <memory>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

class DecryptionLine : public QWidget {
Q_OBJECT

public:
  explicit DecryptionLine(QWidget *parent = nullptr, const QString &path = "");
  ~DecryptionLine() override;
private:
  std::unique_ptr<QVBoxLayout> DecryptionLineLayout;
  std::unique_ptr<QLabel> PathLabel;
  std::unique_ptr<QCheckBox> SelectPath;
  std::shared_ptr<QLabel> DecryptionStatus;
};

#endif //DECRYPTIONLINE
