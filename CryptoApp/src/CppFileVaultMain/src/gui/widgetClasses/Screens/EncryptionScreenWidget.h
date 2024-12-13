#ifndef ENCRYPTIONSCREENWIDGET_H
#define ENCRYPTIONSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class EncryptionScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit EncryptionScreenWidget(QWidget *parent = nullptr);
  ~EncryptionScreenWidget() override;
private:
  QVBoxLayout *EncryptionScreenWidgetLayout;

};

#endif //ENCRYPTIONSCREENWIDGET
