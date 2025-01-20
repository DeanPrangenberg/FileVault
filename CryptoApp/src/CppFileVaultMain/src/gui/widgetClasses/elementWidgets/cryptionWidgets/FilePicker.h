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
#include "../../../../shared/src/GlobalDefinitions.h"

namespace fs = std::filesystem;

/**
 * @brief Enum class for file picker type.
 */
enum class FilePickerType {
  ENCRYPT, ///< File picker for encryption.
  DECRYPT  ///< File picker for decryption.
};

/**
 * @brief Struct for encryption configuration.
 */
struct encryptionConfig {
  bool checkForEnc; ///< Flag to check for encryption.
  QString Algorithm; ///< Algorithm used for encryption.
  QString Path; ///< Path of the file to be encrypted.
};

/**
 * @brief Struct for decryption configuration.
 */
struct decryptionConfig {
  bool checkForDec; ///< Flag to check for decryption.
  QString Path; ///< Path of the file to be decrypted.
};

/**
 * @brief The FilePicker class provides a widget for selecting files or directories for encryption or decryption.
 */
class FilePicker : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Constructs the FilePicker object.
   * @param parent The parent widget.
   * @param type The type of file picker (ENCRYPT or DECRYPT).
   */
  explicit FilePicker(QWidget *parent = nullptr, FilePickerType type = FilePickerType::ENCRYPT);

  /**
   * @brief Destructor for the FilePicker object.
   */
  ~FilePicker() override = default;

  std::unique_ptr<QVBoxLayout> FilePickerLayout; ///< Layout for the file picker.
  std::unique_ptr<QHBoxLayout> FilePickerTopLayout; ///< Top layout for the file picker.
  std::unique_ptr<QHBoxLayout> GlobalControlLayout; ///< Global control layout for the file picker.
  std::unique_ptr<QLabel> BackgroundLabel; ///< Background label for the file picker.
  std::shared_ptr<QPushButton> AddFileButton; ///< Button to add a file.
  std::shared_ptr<QPushButton> AddDirectoryButton; ///< Button to add a directory.
  std::shared_ptr<QPushButton> ResetPathsButton; ///< Button to reset paths.
  std::unique_ptr<QLabel> FileToProcessLabel; ///< Label to show the number of files to process.
  std::unique_ptr<QCheckBox> GlobalCheckBox; ///< Global checkbox to select all files.
  std::unique_ptr<QComboBox> GlobalAlgorithmComboBox; ///< Global combo box to select the algorithm.

  std::unique_ptr<QScrollArea> FilePickerScrollArea; ///< Scroll area for the file picker.
  std::unique_ptr<QWidget> FilePickerScrollAreaWidget; ///< Widget inside the scroll area.
  std::unique_ptr<QVBoxLayout> FilePickerScrollAreaLayout; ///< Layout inside the scroll area.

  FileScannerDLL fileScanner; ///< File scanner for scanning directories.
  QVector<QString> FilePickerPaths; ///< Vector of file picker paths.
  std::vector<fs::path> FileScannerPaths; ///< Vector of file scanner paths.

  /**
   * @brief Configures the user interface for the file picker.
   */
  void configureUI();

  /**
   * @brief Opens a file dialog to add files to the file picker.
   */
  void addFile();

  /**
   * @brief Opens a directory dialog to add a directory to the file picker.
   */
  void addDirectory();

  /**
   * @brief Resets the paths in the file picker.
   */
  void resetPaths();

  /**
   * @brief Updates the label showing the number of files to process.
   */
  void updateFileToProcessLabel();

  /**
   * @brief Handles the state change of the global checkbox.
   * @param state The new state of the checkbox.
   */
  void globalCheckStateChanged(int state);

  /**
   * @brief Handles the change of the global algorithm combo box.
   * @param algorithm The new algorithm selected.
   */
  void globalAlgorithmChanged(const QString &algorithm);

  /**
   * @brief Gets the encryption items from the file picker.
   * @return A vector of encryption configurations.
   */
  QVector<encryptionConfig> getEncItems();

  /**
   * @brief Gets the decryption items from the file picker.
   * @return A vector of decryption configurations.
   */
  QVector<decryptionConfig> getDecItems();

  /**
   * @brief Removes the encryption items from the file picker.
   * @param results The results of the encryption process.
   */
  void removeEncItems(const std::vector<int>& results);

  /**
   * @brief Removes the decryption items from the file picker.
   * @param results The results of the decryption process.
   */
  void removeDecItems(const std::vector<int>& results);

private:
  FilePickerType filePickerType; ///< Type of the file picker (ENCRYPT or DECRYPT).
  QSet<QString> addedPaths; ///< Set of added paths.
  StyleSetter styleSetter; ///< Style setter for the file picker.

  /**
   * @brief Adds an entry to the file picker.
   * @param path The path of the file or directory to add.
   */
  void addEntry(const QString &path);

  /**
   * @brief Scans a directory in a separate thread.
   * @param directory The directory to scan.
   */
  void scanDirectoryInThread(const QString &directory);

  /**
   * @brief Removes items from the file picker based on the results.
   * @param results The results of the process.
   * @param isEncryption Whether the process is encryption or decryption.
   */
  void removeItems(const std::vector<int>& results, bool isEncryption);
};

#endif // FILEVAULTROOT_FILEPICKER_H