#ifndef ENCRYPTIONLINE_H
#define ENCRYPTIONLINE_H

#include <QWidget>
#include <memory>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>

class EncryptionLine : public QWidget {
Q_OBJECT

public:
  explicit EncryptionLine(QWidget *parent = nullptr, const QString &path = "");
  ~EncryptionLine() override;
private:
  std::unique_ptr<QVBoxLayout> EncryptionLineLayout;
  std::unique_ptr<QLabel> PathLabel;
  std::unique_ptr<QCheckBox> SelectPath;
  std::shared_ptr<QLabel> EncryptionStatus;
};

#endif //ENCRYPTIONLINE
