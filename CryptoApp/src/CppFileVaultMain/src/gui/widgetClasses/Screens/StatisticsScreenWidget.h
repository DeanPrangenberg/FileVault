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

class StatisticsScreenWidget : public QWidget {
Q_OBJECT

public:
  explicit StatisticsScreenWidget(QWidget *parent = nullptr);
  ~StatisticsScreenWidget() override = default;

  int aes128Count;
  int aes256Count;
  int lostFilesCount;
  int FoundFilesCount;
  int decryptedFilesCount;
  int encryptedFilesCount;
  int databaseSize;
  int lifetimeAes128Count;
  int lifetimeAes256Count;
  QMap<QString, int> algoFileCounts;

  void updateAes128Count(int amount);
  void updateAes256Count(int amount);
  void updateLostFilesCount(int amount);
  void updateFoundFilesCount(int amount);
  void updateDecryptedFilesCount(int amount);
  void updateEncryptedFilesCount(int amount);

private:
  std::unique_ptr<CakeDiagram> algoCakeDiagram;
  std::unique_ptr<CakeDiagram> LostFilesCakeDiagram;
  std::unique_ptr<QLabel> titleLabel;
  std::unique_ptr<QPushButton> checkFileStateButton;
  std::unique_ptr<QPushButton> tryRepairFileButton;
  std::unique_ptr<QPushButton> saveStatisticsButton;
  std::unique_ptr<QPushButton> loadStatisticsButton;

  std::unique_ptr<QGridLayout> infoTextsLayout;
  std::unique_ptr<AlgorithmStatisticWidget> algoStatsWidget;
  std::unique_ptr<QLabel> decryptedFilesLabel;
  std::unique_ptr<QLabel> encryptedFilesLabel;
  std::unique_ptr<QLabel> sizeOfDatabaseLabel;

private slots:
  void updateStatistics();
  void SetupUI();
  void onSaveStatisticsButtonClicked();
  void onLoadStatisticsButtonClicked();
  void saveStatisticsToJson(const QString &filePath);
  void loadStatisticsFromJson(const QString &filePath);
  void ensureStatisticsFileExists(const QString &filePath);
};

#endif //STATISTICSSCREENWIDGET_H