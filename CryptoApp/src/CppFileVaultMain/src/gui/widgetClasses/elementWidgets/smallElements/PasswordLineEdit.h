#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <memory>
#include "../../../styleClass/StyleSetter.h"

class PasswordWidget : public QWidget {
Q_OBJECT

public:
  std::unique_ptr<QHBoxLayout> mainLayout;
  std::unique_ptr<QLineEdit> passwordField;
  std::unique_ptr<QPushButton> toggleButton;

  explicit PasswordWidget(QWidget *parent = nullptr) : QWidget(parent) {
    setMinimumSize(80, 25);

    mainLayout = std::make_unique<QHBoxLayout>();
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->setSpacing(0);

    passwordField = std::make_unique<QLineEdit>(this);
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    passwordField->setFrame(false);

    toggleButton = std::make_unique<QPushButton>(this);
    toggleButton->setIcon(QIcon(":/icons/Eye.png"));
    toggleButton->setFixedSize(25, 25);

    StyleSetter styleSetter;
    styleSetter.setPasswordToggleButtonStyle(toggleButton.get());
    styleSetter.setLineEditStyle(passwordField.get());

    mainLayout->addWidget(passwordField.get());
    mainLayout->addWidget(toggleButton.get());

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addLayout(mainLayout.get());

    connect(toggleButton.get(), &QPushButton::clicked, this, &PasswordWidget::togglePasswordVisibility);
  }

  ~PasswordWidget() override = default;

  void togglePasswordVisibility() {
    if (passwordField->echoMode() == QLineEdit::Password) {
      passwordField->setEchoMode(QLineEdit::Normal);
      toggleButton->setIcon(QIcon(":/icons/NoEye.png"));
    } else {
      passwordField->setEchoMode(QLineEdit::Password);
      toggleButton->setIcon(QIcon(":/icons/Eye.png"));
    }
  }

  QSize sizeHint() const override {
    QFontMetrics fm(passwordField->font());
    int width = fm.horizontalAdvance(passwordField->text()) + toggleButton->width() + 10; // 10 for margins
    int height = passwordField->sizeHint().height();
    return QSize(width, height);
  }

  QSize minimumSizeHint() const override {
    return QSize(80, 35);
  }

protected:
  void resizeEvent(QResizeEvent *event) override {
    QWidget::resizeEvent(event);
    int newHeight = passwordField->sizeHint().height();
    passwordField->setFixedHeight(newHeight);
  }
};

#endif // PASSWORDLINEEDIT_H