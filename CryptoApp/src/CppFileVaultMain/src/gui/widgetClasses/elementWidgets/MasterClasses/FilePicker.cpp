#include "FilePicker.h"

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

  loadingWindow = std::make_unique<LoadingWindow>(this); // Initialize LoadingWindow
  threadPool = std::make_unique<ThreadPool>(4); // Initialize ThreadPool with 4 threads
}

void FilePicker::configureUI() {
  FilePickerLayout->addLayout(FilePickerTopLayout.get());
  FilePickerTopLayout->addWidget(AddFileButton.get());
  FilePickerTopLayout->addWidget(AddDirectoryButton.get());
  FilePickerTopLayout->addWidget(ResetPathsButton.get());
  FilePickerTopLayout->addWidget(FileToProcessLabel.get());

  if (filePickerType == FilePickerType::ENCRYPT) {
    GlobalAlgorithmComboBox->addItems({"AES-128", "AES-256"});
    GlobalControlLayout->addWidget(GlobalAlgorithmComboBox.get());
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
  styleSetter.setComboBoxStyle(GlobalAlgorithmComboBox.get());
  styleSetter.setScrollAreaStyle(FilePickerScrollArea.get());

  connect(AddFileButton.get(), &QPushButton::clicked, this, &FilePicker::addFile);
  connect(AddDirectoryButton.get(), &QPushButton::clicked, this, &FilePicker::addDirectory);
  connect(ResetPathsButton.get(), &QPushButton::clicked, this, &FilePicker::resetPaths);
  connect(GlobalCheckBox.get(), &QCheckBox::stateChanged, this, &FilePicker::globalCheckStateChanged);
  connect(GlobalAlgorithmComboBox.get(), &QComboBox::currentTextChanged, this, &FilePicker::globalAlgorithmChanged);
}

void FilePicker::addFile() {
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select Files"), "", tr("All Files (*)"));

  for (const QString &fileName : fileNames) {
    qDebug() << "Trying to add file:" << fileName;
    if (!addedPaths.contains(fileName)) {
      addedPaths.insert(fileName);
      addEntry(fileName);
    } else {
      qDebug() << "File already added:" << fileName;
    }
  }
  updateFileToProcessLabel();
}

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
    entryLayout->addWidget(comboBox);
  }

  FilePickerScrollAreaLayout->addLayout(entryLayout);
  qDebug() << "Added entry for path:" << path;

  // Connect the checkbox state change to updateFileToProcessLabel
  connect(checkbox, &QCheckBox::stateChanged, this, &FilePicker::updateFileToProcessLabel);

  updateFileToProcessLabel();
  FilePickerScrollAreaWidget->update(); // Force widget to redraw
}

void FilePicker::convertPathsToQVector(const std::vector<fs::path>& paths, QVector<QString>& output) {
  for (const auto &path : paths) {
    output.push_back(QString::fromStdWString(path.wstring()));
  }
}

void FilePicker::convertBackToPaths(const QVector<QString>& input, std::vector<fs::path>& output) {
  for (const auto &path : input) {
    output.push_back(path.toStdWString());
  }
}

void FilePicker::scanDirectoryInThread(const QString &directory) {
  loadingWindow->show(); // Show LoadingWindow
  threadPool->addTask([this, directory]() {
    fs::path dirPath = directory.toStdWString();
    if (filePickerType == FilePickerType::ENCRYPT) {
      FileScannerPaths = fileScanner.ScanDirectory(dirPath, false);
    } else {
      FileScannerPaths = fileScanner.ScanDirectory(dirPath, true);
    }

    convertPathsToQVector(FileScannerPaths, FilePickerPaths);

    for (const auto &path : FilePickerPaths) {
      if (!addedPaths.contains(path)) {
        addedPaths.insert(path);
        QMetaObject::invokeMethod(this, [this, path]() { addEntry(path); }, Qt::QueuedConnection);
      }
    }

    FileScannerPaths.clear();
    FilePickerPaths.clear();

    emit threadPool->updateGuiSignal("Scanning completed");
    QMetaObject::invokeMethod(loadingWindow.get(), "hide", Qt::QueuedConnection); // Hide LoadingWindow when done
  });
}

void FilePicker::appendConsoleOutput(const QString &text) {
  loadingWindow->appendConsoleOutput(text);
}