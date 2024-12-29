#include "FileVaultGui.h"
#include "widgetClasses/Screens/StatisticsScreenWidget.h"
#include "widgetClasses/Screens/SettingsScreenWidget.h"
#include "widgetClasses/Screens/DecryptionScreenWidget.h"
#include "widgetClasses/Screens/EncryptionScreenWidget.h"

FileVaultGui::FileVaultGui(QMainWindow *parent) : QMainWindow(parent) {
  setWindowTitle("FileVault");
  setMinimumSize(screenWidth, screenHeight);

  centralWidget = std::make_unique<QWidget>(this);
  setCentralWidget(centralWidget.get());

  SplitterLayout = std::make_unique<QHBoxLayout>(centralWidget.get());

  sidebar = std::make_unique<QWidget>(this);
  sideBarLayout = std::make_unique<QVBoxLayout>(sidebar.get());
  Logo = std::make_unique<QLabel>();
  StatisticsSwitch = std::make_unique<QPushButton>(this);
  EncryptSwitch = std::make_unique<QPushButton>(this);
  DecryptSwitch = std::make_unique<QPushButton>(this);
  SettingsSwitch = std::make_unique<QPushButton>(this);

  screenStack = std::make_unique<QStackedWidget>(this);

  setupUi();
}

FileVaultGui::~FileVaultGui() = default;

void FileVaultGui::setupUi() {
  SplitterLayout->addWidget(sidebar.get());
  SplitterLayout->addWidget(screenStack.get());

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

  qDebug() << "FileVaultMainGui: Set button text";

  // Adding buttons to the sidebar
  StatisticsSwitch->setText("Statistics");
  EncryptSwitch->setText("Encrypt");
  DecryptSwitch->setText("Decrypt");

  qDebug() << "FileVaultMainGui: Adding buttons to the sidebar";
  sideBarLayout->addWidget(StatisticsSwitch.get());
  sideBarLayout->addWidget(EncryptSwitch.get());
  sideBarLayout->addWidget(DecryptSwitch.get());
  sideBarLayout->addStretch();

  SettingsSwitch->setText("Settings");
  sideBarLayout->addWidget(SettingsSwitch.get());

  connect(StatisticsSwitch.get(), &QPushButton::clicked, this, [this] { switchScreens("Statistics"); });
  connect(EncryptSwitch.get(), &QPushButton::clicked, this, [this] { switchScreens("Encrypt"); });
  connect(DecryptSwitch.get(), &QPushButton::clicked, this, [this] { switchScreens("Decrypt"); });
  connect(SettingsSwitch.get(), &QPushButton::clicked, this, [this] { switchScreens("Settings"); });
}

void FileVaultGui::setupStackedWidget() {
  qDebug() << "FileVaultMainGui: Setting up screen hashmap";
  auto statisticsScreen = std::make_shared<StatisticsScreenWidget>();
  // Add the screens to the screenHash
  screenHash.insert("Statistics", statisticsScreen);
  qDebug() << "FileVaultMainGui: Added statistics screen";
  screenHash.insert("Encrypt", std::make_shared<EncryptionScreenWidget>(this, statisticsScreen));
  qDebug() << "FileVaultMainGui: Added encryption screen";
  screenHash.insert("Decrypt", std::make_shared<DecryptionScreenWidget>(this, statisticsScreen));
  qDebug() << "FileVaultMainGui: Added decryption screen";
  screenHash.insert("Settings", std::make_shared<SettingsScreenWidget>());
  qDebug() << "FileVaultMainGui: Added settings screen";

  qDebug() << "FileVaultMainGui: Setting up the stacked widget";
  // Add the screens to the stacked widget
  screenStack->addWidget(screenHash["Statistics"].get());
  screenStack->addWidget(screenHash["Encrypt"].get());
  screenStack->addWidget(screenHash["Decrypt"].get());
  screenStack->addWidget(screenHash["Settings"].get());

  qDebug() << "FileVaultMainGui: Setting start screen";
  // Set the default screen
  screenStack->setCurrentWidget(screenHash["Statistics"].get());
}

void FileVaultGui::switchScreens(const QString &screenName) {
  screenStack->setCurrentWidget(screenHash[screenName].get());
}
