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

    initConfig();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(CheckTimeToCtrlClock()));
    timer->start(400);

    showClockBody();
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

void TrayMenu::initClockBodyState()
{
    connect(clockBody, &ClockBody::mouseReleaseSignal, this, &TrayMenu::ClockPositionChanged);
    connect(clockBody, &ClockBody::mousePressSignal, this, &TrayMenu::ClockBodyClicked);
    moveClockBody(configInfo.positionX, configInfo.positionY);
}

void TrayMenu::moveClockBody(const int &x, const int &y)
{
    clockBody->move(x, y);
}

void TrayMenu::stopAllAnimation()
{
    logger->info("current state: {}", clockBodyState);
    if (moveAnimation != nullptr)
    {
        moveAnimation->stop();
        delete moveAnimation;
        moveAnimation = nullptr;
    }
    if (opacityAnimation != nullptr)
    {
        opacityAnimation->stop();
        delete opacityAnimation;
        opacityAnimation = nullptr;
    }
}

void TrayMenu::restoreAllAnimation()
{
    switch (clockBodyState)
    {
    case ClockBodyState::CLOCKBODY_SHOW:
        /* code */
        break;
    case ClockBodyState::CLOCKBODY_HIDE:
        /* code */
        break;
    case ClockBodyState::CLOCKBODY_MOVE_TO_SHOW:
        configInfo.positionX += configInfo.clockMoveDistance;
        Config.SetValue("positionX", std::to_string(configInfo.positionX));
        Config.SaveConfig();
        clockBodyState = ClockBodyState::CLOCKBODY_SHOW;
        stopClockMoveAnimation(StateConditionGuard::GUARD_CLOSE);
        break;
    case ClockBodyState::CLOCKBODY_MOVE_TO_HIDE:
        clockBodyState = ClockBodyState::CLOCKBODY_HIDE;
        startClockMoveAnimation(StateConditionGuard::GUARD_CLOSE);
        break;
    default:
        break;
    }
}

void TrayMenu::startClockMoveAnimation(const StateConditionGuard &GUARD)
{
    if ((configInfo.isAnimationSet && (clockBodyState == ClockBodyState::CLOCKBODY_SHOW)) || GUARD)
    {
        logger->info("startClockMoveAnimation");
        stopAllAnimation();
        logger->info("state changed to CLOCKBODY_MOVE_TO_HIDE");
        clockBodyState = ClockBodyState::CLOCKBODY_MOVE_TO_HIDE;
        moveAnimation = new QPropertyAnimation(clockBody, "pos");
        moveAnimation->setDuration(configInfo.clockMoveSpeed);
        moveAnimation->setStartValue(QPoint(configInfo.positionX, configInfo.positionY));
        moveAnimation->setEndValue(QPoint(configInfo.positionX - configInfo.clockMoveDistance, configInfo.positionY));
        moveAnimation->start();
        connect(moveAnimation, &QPropertyAnimation::finished, [=]()
                {
        logger->info("moveAnimation finished");
        clockBodyState = ClockBodyState::CLOCKBODY_HIDE;
        delete moveAnimation; moveAnimation = nullptr; });

        opacityAnimation = new QPropertyAnimation(clockBody, "windowOpacity");
        opacityAnimation->setDuration(configInfo.clockMoveSpeed);
        opacityAnimation->setKeyValueAt(0, 1);
        opacityAnimation->setKeyValueAt(0.5, 1);
        opacityAnimation->setKeyValueAt(1, 0);
        opacityAnimation->start();
        connect(opacityAnimation, &QPropertyAnimation::finished, [=]()
                {
        logger->info("opacityAnimation finished");
        logger->info("state changed to CLOCKBODY_HIDE");
        clockBodyState = ClockBodyState::CLOCKBODY_HIDE;
        delete opacityAnimation; opacityAnimation = nullptr; });
    }
}

void TrayMenu::stopClockMoveAnimation(const StateConditionGuard &GUARD)
{
    if ((configInfo.isAnimationSet && (clockBodyState == ClockBodyState::CLOCKBODY_HIDE)) || GUARD)
    {

        logger->info("stopClockMoveAnimation");

        stopAllAnimation();

        logger->info("state changed to CLOCKBODY_MOVE_TO_SHOW");
        clockBodyState = ClockBodyState::CLOCKBODY_MOVE_TO_SHOW;
        moveAnimation = new QPropertyAnimation(clockBody, "pos");
        moveAnimation->setDuration(configInfo.clockMoveSpeed);
        moveAnimation->setStartValue(QPoint(configInfo.positionX - configInfo.clockMoveDistance, configInfo.positionY));
        moveAnimation->setEndValue(QPoint(configInfo.positionX, configInfo.positionY));
        moveAnimation->start();
        connect(moveAnimation, &QPropertyAnimation::finished, [=]()
                {
        logger->info("moveAnimation finished");
        clockBodyState = ClockBodyState::CLOCKBODY_SHOW;
        delete moveAnimation;
        moveAnimation = nullptr; });

        opacityAnimation = new QPropertyAnimation(clockBody, "windowOpacity");
        opacityAnimation->setDuration(configInfo.clockMoveSpeed);
        opacityAnimation->setKeyValueAt(0, 0);
        opacityAnimation->setKeyValueAt(0.5, 1);
        opacityAnimation->setKeyValueAt(1, 1);
        opacityAnimation->start();
        connect(opacityAnimation, &QPropertyAnimation::finished, [=]()
                {
        logger->info("opacityAnimation finished");
        logger->info("state changed to CLOCKBODY_SHOW");
        clockBodyState = ClockBodyState::CLOCKBODY_SHOW;
        delete opacityAnimation; 
        opacityAnimation = nullptr; });
    }
}

void TrayMenu::initConfig()
{
    if (Config.IsConfigFirstLoad())
    {
        logger->info("[initConfig] first load");
        saveAllConfig();
    }
    else
    {
        logger->info("[initConfig] not first load");
        configInfo.positionX = QString::fromStdString(Config.GetValue("positionX")).toInt();
        configInfo.positionY = QString::fromStdString(Config.GetValue("positionY")).toInt();
        configInfo.isAutoStartSet = QString::fromStdString(Config.GetValue("isAutoStartSet")).toInt();
        configInfo.isAnimationSet = QString::fromStdString(Config.GetValue("isAnimationSet")).toInt();
        configInfo.clockMoveSpeed = QString::fromStdString(Config.GetValue("clockMoveSpeed")).toInt();
        configInfo.clockMoveInterval = QString::fromStdString(Config.GetValue("clockMoveInterval")).toInt();
        configInfo.clockMoveDistance = QString::fromStdString(Config.GetValue("clockMoveDistance")).toInt();
        configInfo.secondList = QString::fromStdString(Config.GetValue("secondList"));
        configInfo.minuteList = QString::fromStdString(Config.GetValue("minuteList"));
        configInfo.hourList = QString::fromStdString(Config.GetValue("hourList"));
        configInfo.dayList = QString::fromStdString(Config.GetValue("dayList"));
    }
    logger->info("[initConfig] positionX: " + std::to_string(configInfo.positionX));
    logger->info("[initConfig] positionY: " + std::to_string(configInfo.positionY));
    logger->info("[initConfig] isAutoStartSet: " + std::to_string(configInfo.isAutoStartSet));
    logger->info("[initConfig] isAnimationSet: " + std::to_string(configInfo.isAnimationSet));
    logger->info("[initConfig] clockMoveSpeed: " + std::to_string(configInfo.clockMoveSpeed));
    logger->info("[initConfig] clockMoveInterval: " + std::to_string(configInfo.clockMoveInterval));
    logger->info("[initConfig] clockMoveDistance: " + std::to_string(configInfo.clockMoveDistance));
    logger->info("[initConfig] secondList: " + configInfo.secondList.toStdString());
    logger->info("[initConfig] minuteList: " + configInfo.minuteList.toStdString());
    logger->info("[initConfig] hourList: " + configInfo.hourList.toStdString());
    logger->info("[initConfig] dayList: " + configInfo.dayList.toStdString());

    connect(this, &TrayMenu::settingInfoSignal, settingMenu, &SettingMenu::setSettingStructSlot);
    connect(settingMenu, &SettingMenu::settingInfoSignal, this, &TrayMenu::settingInfoSlot);
    emit settingInfoSignal({configInfo.isAutoStartSet, configInfo.isAnimationSet, configInfo.clockMoveSpeed, configInfo.clockMoveInterval, configInfo.clockMoveDistance, configInfo.secondList, configInfo.minuteList, configInfo.hourList, configInfo.dayList});
    SetAutoStart();
    initTime();
    startClockMoveAnimation(StateConditionGuard::GUARD_OPEN);
    initClockBodyState();
}

void TrayMenu::saveAllConfig()
{
    Config.SetValue("positionX", std::to_string(configInfo.positionX));
    Config.SetValue("positionY", std::to_string(configInfo.positionY));
    Config.SetValue("isAutoStartSet", std::to_string(configInfo.isAutoStartSet));
    Config.SetValue("isAnimationSet", std::to_string(configInfo.isAnimationSet));
    Config.SetValue("clockMoveSpeed", std::to_string(configInfo.clockMoveSpeed));
    Config.SetValue("clockMoveInterval", std::to_string(configInfo.clockMoveInterval));
    Config.SetValue("clockMoveDistance", std::to_string(configInfo.clockMoveDistance));
    Config.SetValue("secondList", configInfo.secondList.toStdString());
    Config.SetValue("minuteList", configInfo.minuteList.toStdString());
    Config.SetValue("hourList", configInfo.hourList.toStdString());
    Config.SetValue("dayList", configInfo.dayList.toStdString());
    Config.SaveConfig();
}

void TrayMenu::ClockPositionChanged(QPoint pos)
{
    logger->info("[ClockPositionChanged] pos.x: " + std::to_string(pos.x()));
    logger->info("[ClockPositionChanged] pos.y: " + std::to_string(pos.y()));
    configInfo.positionX = pos.x();
    configInfo.positionY = pos.y();
    Config.SetValue("positionX", std::to_string(configInfo.positionX));
    Config.SetValue("positionY", std::to_string(configInfo.positionY));
    Config.SaveConfig();

    restoreAllAnimation();
}

void TrayMenu::ClockBodyClicked()
{
    logger->info("[ClockBodyClicked]");
    if (configInfo.isAnimationSet)
    {
        stopAllAnimation();
    }
}

void TrayMenu::settingInfoSlot(SettingStruct sets)
{
    logger->info("[settingInfoSlot] get setting info");
    configInfo.isAutoStartSet = sets.isAutoStartSet;
    configInfo.isAnimationSet = sets.isAnimationSet;
    configInfo.clockMoveSpeed = sets.clockMoveSpeed;
    configInfo.clockMoveInterval = sets.clockMoveInterval;
    configInfo.clockMoveDistance = sets.clockMoveDistance;
    configInfo.secondList = sets.secondList;
    configInfo.minuteList = sets.minuteList;
    configInfo.hourList = sets.hourList;
    configInfo.dayList = sets.dayList;
    saveAllConfig();
    SetAutoStart();
    SetAnimation();
    initTime();
}

void TrayMenu::SetAutoStart()
{
    logger->info("[SetAutoStart] set auto start {}", configInfo.isAutoStartSet ? "true" : "false");
    QSettings settings(AUTO_RUN, QSettings::NativeFormat);
    QFileInfo fInfo(QApplication::applicationFilePath());
    QString name = fInfo.baseName();
    QString oldPath = settings.value(name).toString();
    QString newPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    if (configInfo.isAutoStartSet)
    {
        if (oldPath != newPath)
        {
            settings.setValue(name, newPath);
        }
    }
    else
    {
        settings.remove(name);
    }
}

void TrayMenu::SetAnimation()
{
    logger->info("[SetAnimation] set animation {}", configInfo.isAnimationSet ? "true" : "false");
    if (!configInfo.isAnimationSet)
    {
        if (clockBodyState == CLOCKBODY_HIDE || clockBodyState == CLOCKBODY_MOVE_TO_HIDE)
        {
            stopClockMoveAnimation(StateConditionGuard::GUARD_CLOSE);
        }
    }
}

void TrayMenu::initTime()
{
    logger->info("[initTime] init time");
    timeStringToTimeList(configInfo.secondList, SecondList, 60);
    timeStringToTimeList(configInfo.minuteList, MinuteList, 60);
    timeStringToTimeList(configInfo.hourList, HourList, 24);
    timeStringToTimeList(configInfo.dayList, WeekList, 7);

    for (auto i = WeekList->begin(); i != WeekList->end(); i++)
    {
        for (auto j = HourList->begin(); j != HourList->end(); j++)
        {
            for (auto k = MinuteList->begin(); k != MinuteList->end(); k++)
            {
                for (auto l = SecondList->begin(); l != SecondList->end(); l++)
                {
                    QString time = *i + " " + *j + ":" + *k + ":" + *l;
                    logger->debug("[initTime] time: " + time.toStdString());
                }
            }
        }
    }
}

void TrayMenu::timeStringToTimeList(QString timeString, QList<QString> *timeList, const int &maxNum)
{
    timeList->clear();
    if (timeString.isEmpty())
    {
        for (size_t i = 0; i < maxNum; i++)
        {
            if (7 == maxNum)
            {
                timeList->append(findWeekDayName(i));
            }
            else
            {
                timeList->append("xx");
                return;
            }
        }
    }
    else
    {
        QStringList timeListString = timeString.split(" ");
        for (auto i = timeListString.begin(); i != timeListString.end(); i++)
        {
            if (7 == maxNum)
            {
                timeList->append(findWeekDayName((*i).toInt()));
            }
            else
            {
                timeList->push_back(*i);
            }
        }
    }
}

void TrayMenu::CheckTimeToCtrlClock()
{
    if (configInfo.isAnimationSet)
    {
        QDateTime current_date_time = QDateTime::currentDateTime();
        logger->debug("[CheckTimeToCtrlClock] check time to control clock = {}", current_date_time.toString("dddd hh:mm:ss").toStdString());
        int curtime = current_date_time.toSecsSinceEpoch();
        QString current_week = current_date_time.toString("dddd");
        QString current_hour = current_date_time.toString("hh");
        QString current_minute = current_date_time.toString("mm");
        QString current_second = current_date_time.toString("ss");

        for (auto k = WeekList->begin(); k != WeekList->end(); k++)
        {
            logger->debug("current_week = {}, find = {}", current_week.toStdString(), findWeekDayName((*k).toInt()).toStdString());
            if (current_week == *k)
            {
                logger->debug("time to move clock | current_week");
                for (auto j = HourList->begin(); j != HourList->end(); j++)
                {
                    if (current_hour == *j || "xx" == *j)
                    {
                        logger->debug("time to move clock | current_hour");
                        for (auto l = MinuteList->begin(); l != MinuteList->end(); l++)
                        {
                            if (current_minute == *l || "xx" == *l)
                            {
                                logger->debug("time to move clock | current_minute");
                                for (auto i = SecondList->begin(); i != SecondList->end(); i++)
                                {

                                    if (current_second == *i || "xx" == *i)
                                    {
                                        logger->debug("time to move clock = {}", current_date_time.toString("dddd hh:mm:ss").toStdString());
                                        stopClockMoveAnimation(StateConditionGuard::GUARD_OPEN);
                                        finishTime = curtime + configInfo.clockMoveInterval;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (curtime == finishTime)
        {
            logger->info("time to move clock back");
            startClockMoveAnimation(StateConditionGuard::GUARD_OPEN);
        }
    }
}
