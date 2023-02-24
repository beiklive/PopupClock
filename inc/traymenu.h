#ifndef TRAYMENU_H
#define TRAYMENU_H



#include "common.h"
#include "clockbody.h"
#include "settingmenu.h"

extern std::shared_ptr<spdlog::logger> logger;
extern ConfigManager& Config;
class TrayMenu : public QWidget
{
    Q_OBJECT

public:
    TrayMenu(QWidget *parent = nullptr);
    ~TrayMenu();

public:
    void initTrayMenu();
    void initWidget();
    void initTrayIcon();
    void initTrayAction();
    void initTrayMenuAction();

//slots:
    void showSettingMenu();
    void showClockBody();
    void quitApp();

private:
    const QString ICON_PATH = "://DDJ.ico";
    const QString ICON_TOOLTIP = "桀哥的时钟 仿";
    const QString SETTING_ACTION_NAME = "设置";
    const QString QUIT_ACTION_NAME = "退出"; 


private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *showClockBodyAction;
    QAction *showSettingMenuAction;
    QAction *quitAppAction;
    SettingMenu *settingMenu;
    ClockBody *clockBody;
};
#endif // TRAYMENU_H
