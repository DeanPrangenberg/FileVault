#include "FilePicker.h"

/**
 * @brief Constructs the FilePicker object.
 * @param parent The parent widget.
 * @param type The type of file picker (ENCRYPT or DECRYPT).
 */
FilePicker::FilePicker(QWidget *parent, FilePickerType type) : QWidget(parent) {
  filePickerType = type;
  BackgroundLabel = std::make_unique<QLabel>(this);
  FilePickerLayout = std::make_unique<QVBoxLayout>(this);
  FilePickerTopLayout = std::make_unique<QHBoxLayout>();
  GlobalControlLayout = std::make_unique<QHBoxLayout>();

  AddFileButton = std::make_shared<QPushButton>("Add File", this);
  AddDirectoryButton = std::make_shared<QPushButton>("Add Directory", this);
  ResetPathsButton = std::make_shared<QPushButton>("Reset Paths", this);
  FileToProcessLabel = std::make_unique<QLabel>("0/0", this);
  GlobalCheckBox = std::make_unique<QCheckBox>("Select All", this);
  GlobalAlgorithmComboBox = std::make_unique<QComboBox>(this);

  FilePickerScrollArea = std::make_unique<QScrollArea>(this);
  FilePickerScrollAreaWidget = std::make_unique<QWidget>(FilePickerScrollArea.get());
  FilePickerScrollAreaLayout = std::make_unique<QVBoxLayout>(FilePickerScrollAreaWidget.get());

  setLayout(FilePickerLayout.get());
  configureUI();
}

/**
 * @brief Configures the user interface for the file picker.
 */
void FilePicker::configureUI() {
  FilePickerLayout->addLayout(FilePickerTopLayout.get());
  FilePickerTopLayout->addWidget(AddFileButton.get());
  FilePickerTopLayout->addWidget(AddDirectoryButton.get());
  FilePickerTopLayout->addWidget(ResetPathsButton.get());
  FilePickerTopLayout->addWidget(FileToProcessLabel.get());

  if (filePickerType == FilePickerType::ENCRYPT) {
    GlobalAlgorithmComboBox->addItems({"AES-128", "AES-256"});
    GlobalControlLayout->addWidget(GlobalAlgorithmComboBox.get());
  } else {
    GlobalAlgorithmComboBox->hide();
  }

  GlobalControlLayout->addWidget(GlobalCheckBox.get());
  FilePickerLayout->addLayout(GlobalControlLayout.get());

  FilePickerLayout->addWidget(FilePickerScrollArea.get());
  FilePickerScrollArea->setWidget(FilePickerScrollAreaWidget.get());
  FilePickerScrollArea->setWidgetResizable(true);
  FilePickerScrollAreaWidget->setLayout(FilePickerScrollAreaLayout.get());

  // Set the scroll area and its viewport to be transparent
  FilePickerScrollArea->setStyleSheet("background: transparent; border: none;");
  FilePickerScrollArea->viewport()->setStyleSheet("background: transparent;");

  // Align elements to the top
  FilePickerScrollAreaLayout->setAlignment(Qt::AlignTop);

  styleSetter.setButtonStyle(AddFileButton.get());
  styleSetter.setButtonStyle(AddDirectoryButton.get());
  styleSetter.setButtonStyle(ResetPathsButton.get());
  styleSetter.setCheckBoxStyle(GlobalCheckBox.get());
  if (filePickerType == FilePickerType::ENCRYPT) {
    styleSetter.setComboBoxStyle(GlobalAlgorithmComboBox.get());
  }
  styleSetter.setScrollAreaBorderStyle(FilePickerScrollArea.get());

  connect(AddFileButton.get(), &QPushButton::clicked, this, &FilePicker::addFile);
  connect(AddDirectoryButton.get(), &QPushButton::clicked, this, &FilePicker::addDirectory);
  connect(ResetPathsButton.get(), &QPushButton::clicked, this, &FilePicker::resetPaths);
  connect(GlobalCheckBox.get(), &QCheckBox::stateChanged, this, &FilePicker::globalCheckStateChanged);
  if (filePickerType == FilePickerType::ENCRYPT) {
    connect(GlobalAlgorithmComboBox.get(), &QComboBox::currentTextChanged, this, &FilePicker::globalAlgorithmChanged);
  }
}

/**
 * @brief Adds an entry to the file picker.
 * @param path The path of the file or directory to add.
 */
void FilePicker::addEntry(const QString &path) {
  auto entryLayout = new QHBoxLayout();
  auto checkbox = new QCheckBox(this);
  auto label = new QLabel(path, this);

  styleSetter.setCheckBoxStyle(checkbox);
  checkbox->setFixedWidth(35);

  label->setToolTip(path);

  QString displayText = path;
  if (displayText.length() > 80) {
    displayText = displayText.left(25) + " ... " + displayText.right(50);
  }
  label->setText(displayText);

  entryLayout->addWidget(label);
  entryLayout->addWidget(checkbox);

  if (filePickerType == FilePickerType::ENCRYPT) {
    auto comboBox = new QComboBox(this);
    comboBox->setFixedWidth(35);
    styleSetter.setComboBoxStyle(comboBox);
    comboBox->addItems({"AES-128", "AES-256"});
    if (globalDefinitions::stdAlgorithm == "AES-128") {
      comboBox->setCurrentText("AES-128");
    } else {
      comboBox->setCurrentText("AES-256");
    }
    entryLayout->addWidget(comboBox);
  }

  FilePickerScrollAreaLayout->addLayout(entryLayout);
  qDebug() << "Added entry for path:" << path;

  // Connect the checkbox state change to updateFileToProcessLabel
  connect(checkbox, &QCheckBox::checkStateChanged, this, &FilePicker::updateFileToProcessLabel);

  updateFileToProcessLabel();
  FilePickerScrollAreaWidget->update(); // Force widget to redraw
}

/**
 * @brief Opens a file dialog to add files to the file picker.
 */
void FilePicker::addFile() {
  QStringList fileNames;

  if (filePickerType == FilePickerType::DECRYPT) {
    fileNames = QFileDialog::getOpenFileNames(this, tr("Select Files"), "", tr(
        QString("Encrypted Files (*%1)").arg(
            QString::fromStdWString(globalDefinitions::encFileSuffix)).toStdString().c_str()));
  } else if (filePickerType == FilePickerType::ENCRYPT) {
    fileNames = QFileDialog::getOpenFileNames(this, tr("Select Files"), "", tr("All Files (*)"));
  }

  for (const QString &fileName : fileNames) {
    if (!addedPaths.contains(fileName)) {
      addedPaths.insert(fileName);
      addEntry(fileName);
    }
  }
  updateFileToProcessLabel();
}

/**
 * @brief Opens a directory dialog to add a directory to the file picker.
 */
void FilePicker::addDirectory() {
  QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "");

  if (!directory.isEmpty()) {
    qDebug() << "Trying to add directory:" << directory;
    scanDirectoryInThread(directory);
  } else {
    qDebug() << "No directory selected";
  }
  updateFileToProcessLabel();
}

/**
 * @brief Resets the paths in the file picker.
 */
void FilePicker::resetPaths() {
  addedPaths.clear();
  FilePickerPaths.clear();
  FileScannerPaths.clear();

  QLayoutItem* item;
  while ((item = FilePickerScrollAreaLayout->takeAt(0)) != nullptr) {
    while (QLayoutItem* subItem = item->layout()->takeAt(0)) {
      if (subItem->widget()) {
        subItem->widget()->deleteLater();
      }
      delete subItem;
    }
    if (item->widget()) {
      item->widget()->deleteLater();
    }
    delete item;
  }

  updateFileToProcessLabel();
  FilePickerScrollAreaWidget->update(); // Force widget to redraw
}

/**
 * @brief Updates the label showing the number of files to process.
 */
void FilePicker::updateFileToProcessLabel() {
  int totalItems = FilePickerScrollAreaLayout->count();
  int checkedItems = 0;
  for (int i = 0; i < totalItems; ++i) {
    QHBoxLayout* entryLayout = qobject_cast<QHBoxLayout*>(FilePickerScrollAreaLayout->itemAt(i)->layout());
    if (entryLayout) {
      QCheckBox* checkbox = qobject_cast<QCheckBox*>(entryLayout->itemAt(1)->widget());
      if (checkbox && checkbox->isChecked()) {
        ++checkedItems;
      }
    }
  }
  FileToProcessLabel->setText(QString("%1/%2").arg(checkedItems).arg(totalItems));
}

/**
 * @brief Handles the state change of the global checkbox.
 * @param state The new state of the checkbox.
 */
void FilePicker::globalCheckStateChanged(int state) {
  bool isChecked = (state == Qt::Checked);
  int totalItems = FilePickerScrollAreaLayout->count();
  for (int i = 0; i < totalItems; ++i) {
    QHBoxLayout* entryLayout = qobject_cast<QHBoxLayout*>(FilePickerScrollAreaLayout->itemAt(i)->layout());
    if (entryLayout) {
      QCheckBox* checkbox = qobject_cast<QCheckBox*>(entryLayout->itemAt(1)->widget());
      if (checkbox) {
        checkbox->setChecked(isChecked);
      }
    }
  }
  updateFileToProcessLabel();
}

/**
 * @brief Handles the change of the global algorithm combo box.
 * @param algorithm The new algorithm selected.
 */
void FilePicker::globalAlgorithmChanged(const QString &algorithm) {
  int totalItems = FilePickerScrollAreaLayout->count();
  for (int i = 0; i < totalItems; ++i) {
    QHBoxLayout* entryLayout = qobject_cast<QHBoxLayout*>(FilePickerScrollAreaLayout->itemAt(i)->layout());
    if (entryLayout) {
      QComboBox* comboBox = qobject_cast<QComboBox*>(entryLayout->itemAt(2)->widget());
      if (comboBox) {
        comboBox->setCurrentText(algorithm);
      }
    }
  }
}

/**
 * @brief Scans a directory in a separate thread.
 * @param directory The directory to scan.
 */
void FilePicker::scanDirectoryInThread(const QString &directory) {

  // Start a new thread for scanning the directory
  std::thread([this, directory]() {
    fs::path dirPath = directory.toStdWString();
    std::vector<fs::path> scannedPaths;

    fileScanner.ScanDirectory(dirPath, filePickerType == FilePickerType::DECRYPT, scannedPaths, [this](const fs::path &path) {
      QString qPath = QString::fromStdWString(path.wstring());
      if (!addedPaths.contains(qPath)) {
        addedPaths.insert(qPath);
        QMetaObject::invokeMethod(this, [this, qPath]() {
          addEntry(qPath);
        }, Qt::QueuedConnection);
      }
    });
  }).detach(); // Detach the thread to run independently
}

/**
 * @brief Gets the decryption items from the file picker.
 * @return A vector of decryption configurations.
 */
QVector<decryptionConfig> FilePicker::getDecItems() {
  QVector<decryptionConfig> decItems;
  int totalItems = FilePickerScrollAreaLayout->count();
  for (int i = 0; i < totalItems; ++i) {
    QHBoxLayout* entryLayout = qobject_cast<QHBoxLayout*>(FilePickerScrollAreaLayout->itemAt(i)->layout());
    if (entryLayout) {
      QCheckBox* checkbox = qobject_cast<QCheckBox*>(entryLayout->itemAt(1)->widget());
      QLabel* label = qobject_cast<QLabel*>(entryLayout->itemAt(0)->widget());
      if (checkbox && checkbox->isChecked() && label) {
        decryptionConfig config;
        config.checkForDec = checkbox->isChecked();
        config.Path = label->toolTip(); // Use the full path stored in the tooltip
        decItems.append(config);
      }
    }
  }
  return decItems;
}

/**
 * @brief Gets the encryption items from the file picker.
 * @return A vector of encryption configurations.
 */
QVector<encryptionConfig> FilePicker::getEncItems() {
  QVector<encryptionConfig> encItems;
  int totalItems = FilePickerScrollAreaLayout->count();
  for (int i = 0; i < totalItems; ++i) {
    QHBoxLayout* entryLayout = qobject_cast<QHBoxLayout*>(FilePickerScrollAreaLayout->itemAt(i)->layout());
    if (entryLayout) {
      QCheckBox* checkbox = qobject_cast<QCheckBox*>(entryLayout->itemAt(1)->widget());
      QLabel* label = qobject_cast<QLabel*>(entryLayout->itemAt(0)->widget());
      QComboBox* comboBox = qobject_cast<QComboBox*>(entryLayout->itemAt(2)->widget());
      if (checkbox && checkbox->isChecked() && label && comboBox) {
        encryptionConfig config;
        config.checkForEnc = checkbox->isChecked();
        config.Path = label->toolTip(); // Use the full path stored in the tooltip
        config.Algorithm = comboBox->currentText();
        encItems.append(config);
      }
    }
  }
  return encItems;
}

/**
 * @brief Removes the encryption items from the file picker.
 * @param results The results of the encryption process.
 */
void FilePicker::removeEncItems(const std::vector<int>& results) {
  removeItems(results, true);
}

/**
 * @brief Removes the decryption items from the file picker.
 * @param results The results of the decryption process.
 */
void FilePicker::removeDecItems(const std::vector<int>& results) {
  removeItems(results, false);
}

/**
 * @brief Removes items from the file picker based on the results.
 * @param results The results of the process.
 * @param isEncryption Whether the process is encryption or decryption.
 */
void FilePicker::removeItems(const std::vector<int>& results, bool isEncryption) {
  for (auto & res : results) {
    qDebug() << "Result: " << res;
  }
  int totalItems = FilePickerScrollAreaLayout->count();
  for (int i = totalItems - 1; i >= 0; --i) {
    QHBoxLayout* entryLayout = qobject_cast<QHBoxLayout*>(FilePickerScrollAreaLayout->itemAt(i)->layout());
    if (entryLayout) {
      QCheckBox* checkbox = qobject_cast<QCheckBox*>(entryLayout->itemAt(1)->widget());
      QLabel* label = qobject_cast<QLabel*>(entryLayout->itemAt(0)->widget());
      QComboBox* comboBox = isEncryption ? qobject_cast<QComboBox*>(entryLayout->itemAt(2)->widget()) : nullptr;
      if (checkbox && checkbox->isChecked() && label) {
        int result = results[i];
        if (result == -1) {
          // Remove the entry
          while (QLayoutItem* subItem = entryLayout->takeAt(0)) {
            if (subItem->widget()) {
              subItem->widget()->deleteLater();
            }
            delete subItem;
          }
          delete entryLayout;
        } else {
          // Show error message
          QString errorMessage = QString("Error code: %1").arg(QString::number(result));
          label->setToolTip(errorMessage);
        }
      }
    }
  }
  updateFileToProcessLabel();
  FilePickerScrollAreaWidget->update(); // Force widget to redraw
}