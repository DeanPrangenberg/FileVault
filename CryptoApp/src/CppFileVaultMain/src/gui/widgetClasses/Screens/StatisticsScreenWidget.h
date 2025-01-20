#ifndef STATISTICSSCREENWIDGET_H
#define STATISTICSSCREENWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMap>
#include "../elementWidgets/statisticWidgets/CakeDiagram.h"
#include "../elementWidgets/statisticWidgets/AlgorithmStatisticWidget.h"
#include "styleClass/StyleSetter.h"
#include "../../../HelperUtils/HelperUtils.h"
#include "RestAPI/RestApiDLL.h"

/**
 * @class StatisticsScreenWidget
 * @brief A widget to display and manage statistics in the FileVault application.
 */
class StatisticsScreenWidget : public QWidget {
Q_OBJECT

public:
  /**
   * @brief Constructs the StatisticsScreenWidget object.
   * @param parent The parent widget.
   */
  explicit StatisticsScreenWidget(QWidget *parent = nullptr);

  /**
   * @brief Destructor for the StatisticsScreenWidget class.
   */
  ~StatisticsScreenWidget() override = default;

  int aes128Count; ///< Count of AES-128 encrypted files.
  int aes256Count; ///< Count of AES-256 encrypted files.
  int lostFilesCount; ///< Count of lost files.
  int FoundFilesCount; ///< Count of found files.
  int decryptedFilesCount; ///< Count of decrypted files.
  int encryptedFilesCount; ///< Count of encrypted files.
  int databaseSize; ///< Size of the database.
  int lifetimeAes128Count; ///< Lifetime count of AES-128 encrypted files.
  int lifetimeAes256Count; ///< Lifetime count of AES-256 encrypted files.
  QMap<QString, int> algoFileCounts; ///< Map of algorithm names to file counts.

  /**
   * @brief Updates the AES-128 file count.
   * @param amount The amount to update by.
   */
  void updateAes128Count(int amount);

  /**
   * @brief Updates the AES-256 file count.
   * @param amount The amount to update by.
   */
  void updateAes256Count(int amount);

  /**
   * @brief Updates the lost files count.
   * @param amount The amount to update by.
   */
  void updateLostFilesCount(int amount);

  /**
   * @brief Updates the found files count.
   * @param amount The amount to update by.
   */
  void updateFoundFilesCount(int amount);

  /**
   * @brief Updates the decrypted files count.
   * @param amount The amount to update by.
   */
  void updateDecryptedFilesCount(int amount);

  /**
   * @brief Updates the encrypted files count.
   * @param amount The amount to update by.
   */
  void updateEncryptedFilesCount(int amount);

private:
  std::unique_ptr<CakeDiagram> algoCakeDiagram; ///< Diagram for algorithm statistics.
  std::unique_ptr<CakeDiagram> LostFilesCakeDiagram; ///< Diagram for lost files statistics.
  std::unique_ptr<QLabel> titleLabel; ///< Title label.
  std::unique_ptr<QPushButton> checkFileStateButton; ///< Button to check file state.
  std::unique_ptr<QPushButton> tryRepairFileButton; ///< Button to try repairing a file.

  std::unique_ptr<QGridLayout> infoTextsLayout; ///< Layout for information texts.
  std::unique_ptr<AlgorithmStatisticWidget> algoStatsWidget; ///< Widget for algorithm statistics.
  std::unique_ptr<QLabel> decryptedFilesLabel; ///< Label for decrypted files count.
  std::unique_ptr<QLabel> encryptedFilesLabel; ///< Label for encrypted files count.
  std::unique_ptr<QLabel> sizeOfDatabaseLabel; ///< Label for database size.

  std::unique_ptr<HelperUtils> helperUtils; ///< Helper utilities.

private slots:
  /**
   * @brief Updates the statistics displayed in the widget.
   */
  void updateStatistics();

  /**
   * @brief Sets up the user interface for the widget.
   */
  void SetupUI();

  /**
   * @brief Slot for handling save statistics button click.
   */
  void onSaveStatisticsButtonClicked();

  /**
   * @brief Slot for handling load statistics button click.
   */
  void onLoadStatisticsButtonClicked();

  /**
   * @brief Saves the statistics to a JSON file.
   * @param filePath The path to the JSON file.
   */
  void saveStatisticsToJson(const QString &filePath);

  /**
   * @brief Loads the statistics from a JSON file.
   * @param filePath The path to the JSON file.
   */
  void loadStatisticsFromJson(const QString &filePath);

  /**
   * @brief Ensures that the statistics file exists.
   * @param filePath The path to the statistics file.
   */
  void ensureStatisticsFileExists(const QString &filePath);
};

#endif //STATISTICSSCREENWIDGET_H