#include "FileVaultGui.h"
#include "widgetClasses/Screens/StatisticsScreenWidget.h"
#include "widgetClasses/Screens/SettingsScreenWidget.h"
#include "widgetClasses/Screens/DecryptionScreenWidget.h"
#include "widgetClasses/Screens/EncryptionScreenWidget.h"

FileVaultGui::FileVaultGui(QMainWindow *parent) : QMainWindow(parent) {
  setFixedSize(screenWidth, screenHeight);

  auto *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  mainLayout = new QVBoxLayout(centralWidget);
  SplitterLayout = new QHBoxLayout();
  screenStack = new QStackedWidget;

  sidebar = new QWidget;
  sideBarLayout = new QVBoxLayout(sidebar);

  Logo = new QLabel();
  StatisticsSwitch = new QPushButton;
  EncryptSwitch = new QPushButton;
  DecryptSwitch = new QPushButton;
  SettingsSwitch = new QPushButton;

  setupUi();
}

FileVaultGui::~FileVaultGui() {
  delete mainLayout;
  delete SplitterLayout;
  delete screenStack;
  delete sidebar;
  delete sideBarLayout;
  delete Logo;
  delete StatisticsSwitch;
  delete EncryptSwitch;
  delete DecryptSwitch;
  delete SettingsSwitch;
}

void FileVaultGui::setupUi() {
  mainLayout->addLayout(SplitterLayout);
  SplitterLayout->addWidget(sidebar);
  SplitterLayout->addWidget(screenStack);

  sidebar->setLayout(sideBarLayout);
  sidebar->setFixedWidth(200);

  // Set radial gradient background for the central widget
  QString gradientStyle = "background: qradialgradient("
                          "cx:0.1, cy:0.2, "
                          "radius:1.0, "
                          "fx:0.1, fy:0.2, "
                          "stop:0 rgba(100, 43, 115, 1), "
                          "stop:0.9 rgba(25, 0, 51, 1));"; // Dark mix of blue and purple
  centralWidget()->setStyleSheet(gradientStyle);

  // Make other widgets transparent
  sidebar->setStyleSheet("background: transparent;");
  screenStack->setObjectName("screenStack");
  screenStack->setStyleSheet("background: transparent;");

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
  styleSetter.setButtonStyle(StatisticsSwitch);
  styleSetter.setButtonStyle(EncryptSwitch);
  styleSetter.setButtonStyle(DecryptSwitch);
  styleSetter.setButtonStyle(SettingsSwitch);

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

  sideBarLayout->addWidget(Logo);
  sideBarLayout->addStretch(); // spacer

  // Adding buttons to the sidebar
  StatisticsSwitch->setText("Statistics");
  EncryptSwitch->setText("Encrypt");
  DecryptSwitch->setText("Decrypt");

  sideBarLayout->addWidget(StatisticsSwitch);
  sideBarLayout->addWidget(EncryptSwitch);
  sideBarLayout->addWidget(DecryptSwitch);
  sideBarLayout->addStretch(); // spacer

  SettingsSwitch->setText("Settings");
  sideBarLayout->addWidget(SettingsSwitch);
}

void FileVaultGui::setupStackedWidget() {
  // Add the screens to the screenHash
  screenHash.insert("Statistics", new StatisticsScreenWidget);
  screenHash.insert("Encrypt", new EncryptionScreenWidget);
  screenHash.insert("Decrypt", new DecryptionScreenWidget);
  screenHash.insert("Settings", new SettingsScreenWidget);

  // Add the screens to the stacked widget
  screenStack->addWidget(screenHash.value("Statistics"));
  screenStack->addWidget(screenHash.value("Encrypt"));
  screenStack->addWidget(screenHash.value("Decrypt"));
  screenStack->addWidget(screenHash.value("Settings"));

  // Set the default screen
  screenStack->setCurrentWidget(screenHash.value("Settings"));
}
