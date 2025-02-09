#ifndef FILEDELETIONWIDGET_H
#define FILEDELETIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include "MasterSettingsWidget.h"

/**
 * @class FileDeletionWidget
 * @brief A widget for handling file deletion settings.
 *
 * This widget provides options to delete files after encryption or decryption.
 */
class FileDeletionWidget : public MasterSettingsWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs a FileDeletionWidget.
   *
   * @param parent The parent widget.
   */
  explicit FileDeletionWidget(QWidget *parent = nullptr);

  /// Checkbox to delete files after encryption.
  std::shared_ptr<QCheckBox> deleteAfterEncryption;

  /// Checkbox to delete files after decryption.
  std::shared_ptr<QCheckBox> deleteAfterDecryption;
};

#endif // FILEDELETIONWIDGET_H