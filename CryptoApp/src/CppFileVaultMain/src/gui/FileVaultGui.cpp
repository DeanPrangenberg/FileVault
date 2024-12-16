#include "FileVaultGui.h"
#include "widgetClasses/Screens/StatisticsScreenWidget.h"
#include "widgetClasses/Screens/SettingsScreenWidget.h"
#include "widgetClasses/Screens/DecryptionScreenWidget.h"
#include "widgetClasses/Screens/EncryptionScreenWidget.h"

FileVaultGui::FileVaultGui(QMainWindow *parent) : QMainWindow(parent) {
  setFixedSize(screenWidth, screenHeight);

  centralWidget = std::make_unique<QWidget>(this);
  setCentralWidget(centralWidget.get());

  mainLayout = std::make_unique<QVBoxLayout>(centralWidget.get());
  SplitterLayout = std::make_unique<QHBoxLayout>();
  screenStack = std::make_unique<QStackedWidget>();

  sidebar = std::make_unique<QWidget>();
  sideBarLayout = std::make_unique<QVBoxLayout>(sidebar.get());

  Logo = std::make_unique<QLabel>();
  StatisticsSwitch = std::make_unique<QPushButton>();
  EncryptSwitch = std::make_unique<QPushButton>();
  DecryptSwitch = std::make_unique<QPushButton>();
  SettingsSwitch = std::make_unique<QPushButton>();

  setupUi();
}

FileVaultGui::~FileVaultGui() = default;

void FileVaultGui::setupUi() {
  mainLayout->addLayout(SplitterLayout.get());
  SplitterLayout->addWidget(sidebar.get());
  SplitterLayout->addWidget(screenStack.get());

  sidebar->setLayout(sideBarLayout.get());
  sidebar->setFixedWidth(200);

  // Set radial gradient background for the central widget
  QString objectName = "centralWidget";
  centralWidget->setObjectName(objectName);
  QString gradientStyle = QString("QWidget#%1 {"
                          "background: qradialgradient("
                          "cx:0.1, cy:0.2, "
                          "radius:1.0, "
                          "fx:0.1, fy:0.2, "
                          "stop:0 rgba(100, 43, 115, 1), "
                          "stop:0.9 rgba(25, 0, 51, 1))}").arg(centralWidget->objectName()); // Dark mix of blue and purple
  centralWidget->setStyleSheet(gradientStyle);

  // Make other widgets transparent
  sidebar->setStyleSheet("background: transparent;");
  screenStack->setObjectName("screenStack");
  screenStack->setStyleSheet(QString("QStackedWidget#%1 { background: transparent;}").arg(screenStack->objectName()));
  StyleSetter styleSetter;
  styleSetter.setWidgetStyle(screenStack.get());

  // Call other setup functions
  setupSideBar();
  setupStackedWidget();
}

void FileVaultGui::setupSideBar() {
  // Adjust button sizes and spacing
  StatisticsSwitch->setMinimumHeight(50);
  EncryptSwitch->setMinimumHeight(50);
  DecryptSwitch->setMinimumHeight(50);
  SettingsSwitch->setMinimumHeight(50);
  sideBarLayout->setSpacing(20);

  // Set button styles using the helper function
  StyleSetter styleSetter;
  styleSetter.setButtonStyle(StatisticsSwitch.get());
  styleSetter.setButtonStyle(EncryptSwitch.get());
  styleSetter.setButtonStyle(DecryptSwitch.get());
  styleSetter.setButtonStyle(SettingsSwitch.get());

  // Add logo to the sidebar
  Logo->setAlignment(Qt::AlignTop);
  Logo->setFixedHeight(100);
  QPixmap logoPixmap(":/logos/LogoWithName.png");
  if (!logoPixmap.isNull()) {
    logoPixmap = logoPixmap.scaled(Logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    Logo->setPixmap(logoPixmap);
    qDebug() << "FileVaultMainGui: Logo pixmap loaded and scaled successfully";
  } else {
    qDebug() << "FileVaultMainGui: Failed to load logo pixmap";
  }

  sideBarLayout->addWidget(Logo.get());
  sideBarLayout->addStretch(); // spacer

  // Adding buttons to the sidebar
  StatisticsSwitch->setText("Statistics");
  EncryptSwitch->setText("Encrypt");
  DecryptSwitch->setText("Decrypt");

  sideBarLayout->addWidget(StatisticsSwitch.get());
  sideBarLayout->addWidget(EncryptSwitch.get());
  sideBarLayout->addWidget(DecryptSwitch.get());
  sideBarLayout->addStretch();

  SettingsSwitch->setText("Settings");
  sideBarLayout->addWidget(SettingsSwitch.get());
}

void FileVaultGui::setupStackedWidget() {
  // Add the screens to the screenHash
  screenHash.insert("Statistics", std::make_shared<StatisticsScreenWidget>());
  screenHash.insert("Encrypt", std::make_shared<EncryptionScreenWidget>());
  screenHash.insert("Decrypt", std::make_shared<DecryptionScreenWidget>());
  screenHash.insert("Settings", std::make_shared<SettingsScreenWidget>());

  // Add the screens to the stacked widget
  screenStack->addWidget(screenHash["Statistics"].get());
  screenStack->addWidget(screenHash["Encrypt"].get());
  screenStack->addWidget(screenHash["Decrypt"].get());
  screenStack->addWidget(screenHash["Settings"].get());

  // Set the default screen
  screenStack->setCurrentWidget(screenHash["Settings"].get());
}