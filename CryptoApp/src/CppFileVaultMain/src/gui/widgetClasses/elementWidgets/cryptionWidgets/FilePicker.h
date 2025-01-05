#ifndef FILEVAULTROOT_FILEPICKER_H
#define FILEVAULTROOT_FILEPICKER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <memory>
#include <vector>
#include <filesystem>
#include <QString>
#include <QVector>
#include <QSet>
#include <QFileDialog>
#include <QFontMetrics>
#include "styleClass/StyleSetter.h"
#include "FileScannerDLL.h"
#include <thread>
#include "../../../../GlobalDefinitions.h"

namespace fs = std::filesystem;

enum class FilePickerType {
  ENCRYPT,
  DECRYPT
};

struct encryptionConfig {
  bool checkForEnc;
  QString Algorithm;
  QString Path;
};

struct decryptionConfig {
  bool checkForDec;
  QString Path;
};

class FilePicker : public QWidget {
  Q_OBJECT

public:
  explicit FilePicker(QWidget *parent = nullptr, FilePickerType type = FilePickerType::ENCRYPT);
  ~FilePicker() override = default;

  std::unique_ptr<QVBoxLayout> FilePickerLayout;
  std::unique_ptr<QHBoxLayout> FilePickerTopLayout;
  std::unique_ptr<QHBoxLayout> GlobalControlLayout;
  std::unique_ptr<QLabel> BackgroundLabel;
  std::shared_ptr<QPushButton> AddFileButton;
  std::shared_ptr<QPushButton> AddDirectoryButton;
  std::shared_ptr<QPushButton> ResetPathsButton;
  std::unique_ptr<QLabel> FileToProcessLabel;
  std::unique_ptr<QCheckBox> GlobalCheckBox;
  std::unique_ptr<QComboBox> GlobalAlgorithmComboBox;

  std::unique_ptr<QScrollArea> FilePickerScrollArea;
  std::unique_ptr<QWidget> FilePickerScrollAreaWidget;
  std::unique_ptr<QVBoxLayout> FilePickerScrollAreaLayout;

  FileScannerDLL fileScanner;
  QVector<QString> FilePickerPaths;
  std::vector<fs::path> FileScannerPaths;

  void configureUI();
  void addFile();
  void addDirectory();
  void resetPaths();
  void updateFileToProcessLabel();
  void globalCheckStateChanged(int state);
  void globalAlgorithmChanged(const QString &algorithm);
  QVector<encryptionConfig> getEncItems();
  QVector<decryptionConfig> getDecItems();
  void removeEncItems(const std::vector<int>& results);
  void removeDecItems(const std::vector<int>& results);

private:
  FilePickerType filePickerType;
  QSet<QString> addedPaths; // Declare addedPaths here
  StyleSetter styleSetter;

  void addEntry(const QString &path);
  void scanDirectoryInThread(const QString &directory); // Add method to scan directory in thread
  void removeItems(const std::vector<int>& results, bool isEncryption);
};

#endif // FILEVAULTROOT_FILEPICKER_H