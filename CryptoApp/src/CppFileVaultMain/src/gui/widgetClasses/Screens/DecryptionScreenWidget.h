#ifndef DECRYPTIONSCREENWIDGET_H
#define DECRYPTIONSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class DecryptionScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit DecryptionScreenWidget(QWidget *parent = nullptr);
  ~DecryptionScreenWidget() override;
private:
  QVBoxLayout *DecryptionScreenWidgetLayout;

};

#endif //DECRYPTIONSCREENWIDGET
