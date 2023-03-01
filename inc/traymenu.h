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
    void appRestart();

// menu init
    void initTrayMenu();
    void initWidget();
    void initTrayIcon();
    void initTrayAction();
    void initTrayMenuAction();

// menu action
    void showSettingMenu();
    void showClockBody();
    void quitApp();

// config
    void initConfig();
    void saveAllConfig();

// control clock
    void animationShowToHide(const StateConditionGuard &GUARD);
    void animationHideToShow(const StateConditionGuard &GUARD);
    void stopAllAnimation();
    void restoreAllAnimation();
    void initClockBodyState();
    void moveClockBody(const int& x, const int& y);

// setting ctrl
    void initTime();
    void SetAutoStart();
    void SetAnimation();
    void timeStringToTimeList(QString timeString, QList<QString> *timeList, const int& maxNum);
    QString findWeekDayName(int index){return Weeks[index];}

// state machine
    void SetClockBodyCurState(const ClockBodyState& state);
    const ClockBodyState GetClockBodyCurState();
    const ClockBodyState GetClockBodyTargetState();

public slots:
    void CheckTimeToCtrlClock();
    void ClockPositionChanged(QPoint pos);
    void ClockBodyClicked();

    void settingInfoSlot(SettingStruct sets);

signals:
    void settingInfoSignal(SettingStruct sets);

private:
    const QString ICON_PATH = "://DDJ.ico";
    const QString ICON_TOOLTIP = "桀哥的时钟 仿";
    const QString SETTING_ACTION_NAME = "设置";
    const QString QUIT_ACTION_NAME = "退出"; 
    const QString AUTO_RUN = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const QString Weeks[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *showClockBodyAction;
    QAction *showSettingMenuAction;
    QAction *quitAppAction;
    QList<QString> *SecondList = new QList<QString>;
    QList<QString> *MinuteList = new QList<QString>;
    QList<QString> *HourList = new QList<QString>;
    QList<QString> *WeekList = new QList<QString>;

    QList<QString> *SecondCloseList = new QList<QString>;
    QList<QString> *MinuteCloseList = new QList<QString>;
    QList<QString> *HourCloseList = new QList<QString>;
    QList<QString> *WeekCloseList = new QList<QString>;
    QPropertyAnimation* moveAnimation = nullptr;
    QPropertyAnimation* opacityAnimation = nullptr;
    SettingMenu *settingMenu;
    ClockBody *clockBody;
    ConfigStruct configInfo{
        30, 70, 3, false, false, 1500, 60, 350, "", "", "", ""
    };
    ClockBodyState clockBodyState = CLOCKBODY_SHOW;
    ClockBodyStateStruct m_clockBodyStateMachine{
        ClockBodyState::CLOCKBODY_SHOW,
        ClockBodyState::CLOCKBODY_MOVE_TO_HIDE
    };
};
#endif // TRAYMENU_H
