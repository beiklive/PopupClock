#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent)
    : QWidget(parent)
{
    initTrayMenu();
}

// void initTrayMenu()
void TrayMenu::initTrayMenu()
{
    logger->info("initTrayMenu");
    initWidget();
    initTrayIcon();
    initTrayAction();
    initTrayMenuAction();

    clockBody->show();
}

// void initWidget()
void TrayMenu::initWidget()
{
    logger->info("initWidget");
    clockBody = new ClockBody();
    settingMenu = new SettingMenu();
}

// void initTrayIcon()
void TrayMenu::initTrayIcon()
{
    logger->info("initTrayIcon");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(ICON_PATH));
    trayIcon->setToolTip(ICON_TOOLTIP);
    trayIcon->show();
}

// void initTrayAction()
void TrayMenu::initTrayAction()
{
    logger->info("initTrayAction");
    // showClockBodyAction = new QAction("Show Clock", this);
    showSettingMenuAction = new QAction(SETTING_ACTION_NAME, this);
    quitAppAction = new QAction(QUIT_ACTION_NAME, this);
}

// void initTrayMenuAction()
void TrayMenu::initTrayMenuAction()
{
    logger->info("initTrayMenuAction");
    trayMenu = new QMenu(this);
    trayMenu->addAction(showSettingMenuAction); 
    trayMenu->addSeparator();
    trayMenu->addAction(quitAppAction);
    trayIcon->setContextMenu(trayMenu);
    connect(showSettingMenuAction, &QAction::triggered, this, &TrayMenu::showSettingMenu);
    connect(quitAppAction, &QAction::triggered, this, &TrayMenu::quitApp);
}

// void showSettingMenu()
void TrayMenu::showSettingMenu()
{
    logger->info("showSettingMenu");
    settingMenu->show();
}

// void showClockBody()
void TrayMenu::showClockBody()
{
    logger->info("showClockBody");
    clockBody->show();
}

// void quitApp()
void TrayMenu::quitApp()
{
    logger->info("quitApp");
    qApp->quit();
}

TrayMenu::~TrayMenu()
{
}

