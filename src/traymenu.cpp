#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent)
    : QWidget(parent)
{
    initTrayMenu();
}
void TrayMenu::appRestart()
{
    logger->info("appRestart");
    const QString program = QCoreApplication::applicationFilePath();
    const QStringList arguments = QCoreApplication::arguments();
    const QString directory = QDir::currentPath();
    QCoreApplication::exit();
    QProcess::startDetached(program, arguments, directory);
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
    timer->start(900);

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
    update();
    SetClockBodyCurState(ClockBodyState::CLOCKBODY_MOVE_TO_HIDE);
    animationShowToHide(StateConditionGuard::GUARD_OPEN);
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
    connect(settingMenu, &SettingMenu::clockSizeSignal, clockBody, &ClockBody::bodySizeSlot);
    moveClockBody(configInfo.positionX, configInfo.positionY);
    clockBody->initLayout(configInfo.size);

}

void TrayMenu::moveClockBody(const int &x, const int &y)
{
    clockBody->move(x, y);
}

void TrayMenu::stopAllAnimation()
{
    logger->info("current state: {}", GetClockBodyCurState());
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
    switch (GetClockBodyCurState())
    {
    case ClockBodyState::CLOCKBODY_SHOW:
        /* code */
        break;
    case ClockBodyState::CLOCKBODY_HIDE:
        /* code */
        break;
    case ClockBodyState::CLOCKBODY_MOVE_TO_SHOW:
        // 因为主坐标是show的坐标，所以在hide或者hide to show的时候，需要把拿到的坐标做一下偏移
        configInfo.positionX += configInfo.clockMoveDistance;
        Config.SetValue("positionX", std::to_string(configInfo.positionX));
        Config.SaveConfig();
        animationHideToShow(StateConditionGuard::GUARD_OPEN);
        break;
    case ClockBodyState::CLOCKBODY_MOVE_TO_HIDE:
        animationShowToHide(StateConditionGuard::GUARD_OPEN);
        break;
    default:
        break;
    }
}

void TrayMenu::animationShowToHide(const StateConditionGuard &GUARD)
{
    if (GUARD || (configInfo.isAnimationSet && GetClockBodyCurState() == ClockBodyState::CLOCKBODY_MOVE_TO_HIDE))
    {
        logger->info("animationShowToHide");
        stopAllAnimation();

        SetClockBodyCurState(ClockBodyState::CLOCKBODY_MOVE_TO_HIDE);

        moveAnimation = new QPropertyAnimation(clockBody, "pos");
        moveAnimation->setDuration(configInfo.clockMoveSpeed);
        moveAnimation->setStartValue(clockBody->pos());
        moveAnimation->setEndValue(QPoint(configInfo.positionX - configInfo.clockMoveDistance, configInfo.positionY));
        moveAnimation->start();
        connect(moveAnimation, &QPropertyAnimation::finished, [=]()
                {
                    logger->info("moveAnimation finished");
                    SetClockBodyCurState(GetClockBodyTargetState());
                    delete moveAnimation;
                    moveAnimation = nullptr; });

        opacityAnimation = new QPropertyAnimation(clockBody, "windowOpacity");
        opacityAnimation->setDuration(configInfo.clockMoveSpeed);
        opacityAnimation->setKeyValueAt(0, 1);
        opacityAnimation->setKeyValueAt(0.5, 1);
        opacityAnimation->setKeyValueAt(1, 0);
        opacityAnimation->start();
        connect(opacityAnimation, &QPropertyAnimation::finished, [=]()
                {
                    delete opacityAnimation;
                    opacityAnimation = nullptr; });
    }
}

void TrayMenu::animationHideToShow(const StateConditionGuard &GUARD)
{
    if (GUARD || (configInfo.isAnimationSet && GetClockBodyCurState() == ClockBodyState::CLOCKBODY_MOVE_TO_SHOW))
    {
        logger->info("animationHideToShow");

        stopAllAnimation();

        SetClockBodyCurState(ClockBodyState::CLOCKBODY_MOVE_TO_SHOW);

        moveAnimation = new QPropertyAnimation(clockBody, "pos");
        moveAnimation->setDuration(configInfo.clockMoveSpeed);
        moveAnimation->setStartValue(clockBody->pos());
        moveAnimation->setEndValue(QPoint(configInfo.positionX, configInfo.positionY));
        moveAnimation->start();
        connect(moveAnimation, &QPropertyAnimation::finished, [=]()
                {
                    logger->info("moveAnimation finished");
                    SetClockBodyCurState(GetClockBodyTargetState());
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
                    delete opacityAnimation;
                    opacityAnimation = nullptr; });
    }
}

// ===================读取配置文件===================
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
        configInfo.size = QString::fromStdString(Config.GetValue("size")).toInt();
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
    logger->info("[initConfig] size: " + std::to_string(configInfo.size));
    logger->info("[initConfig] isAutoStartSet: " + std::to_string(configInfo.isAutoStartSet));
    logger->info("[initConfig] isAnimationSet: " + std::to_string(configInfo.isAnimationSet));
    logger->info("[initConfig] clockMoveSpeed: " + std::to_string(configInfo.clockMoveSpeed));
    logger->info("[initConfig] clockMoveInterval: " + std::to_string(configInfo.clockMoveInterval));
    logger->info("[initConfig] clockMoveDistance: " + std::to_string(configInfo.clockMoveDistance));
    logger->info("[initConfig] secondList: " + configInfo.secondList.toStdString());
    logger->info("[initConfig] minuteList: " + configInfo.minuteList.toStdString());
    logger->info("[initConfig] hourList: " + configInfo.hourList.toStdString());
    logger->info("[initConfig] dayList: " + configInfo.dayList.toStdString());

    // 绑定托盘与设置界面的信号和槽函数
    connect(this, &TrayMenu::settingInfoSignal, settingMenu, &SettingMenu::setSettingStructSlot);
    connect(settingMenu, &SettingMenu::settingInfoSignal, this, &TrayMenu::settingInfoSlot);
    // 发送信号，将配置信息传递给设置界面
    emit settingInfoSignal({configInfo.isAutoStartSet, configInfo.isAnimationSet, configInfo.size, configInfo.clockMoveSpeed, configInfo.clockMoveInterval, configInfo.clockMoveDistance, configInfo.secondList, configInfo.minuteList, configInfo.hourList, configInfo.dayList});
    // 检查是否设置了开机自启
    SetAutoStart();

    // 初始化时钟列表
    initTime();

    // 绑定时钟的信号和槽函数
    initClockBodyState();

    // 播放一个时钟消失的动画，表示时钟初始化完成
    animationShowToHide(StateConditionGuard::GUARD_OPEN);
}

// ==================== 保存配置 ====================
void TrayMenu::saveAllConfig()
{
    Config.SetValue("positionX", std::to_string(configInfo.positionX));
    Config.SetValue("positionY", std::to_string(configInfo.positionY));
    Config.SetValue("size", std::to_string(configInfo.size));
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

// ==============================================================
// 鼠标松开
// 功能： 1. 保存时钟位置 2. 恢复所有动画
// ==============================================================

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

// ==============================================================
// 时钟被点击、时钟被拖动
// 功能： 1. 停止所有动画
// ==============================================================
void TrayMenu::ClockBodyClicked()
{
    logger->info("[ClockBodyClicked]");
    if (configInfo.isAnimationSet)
    {
        stopAllAnimation();
    }
}

// ==============================================================
// 设置槽函数
// 把设置菜单传来的设置信息保存到配置文件中
// 重启程序，刷新配置
// ==============================================================
void TrayMenu::settingInfoSlot(SettingStruct sets)
{
    logger->info("[settingInfoSlot] get setting info");
    configInfo.isAutoStartSet = sets.isAutoStartSet;
    configInfo.isAnimationSet = sets.isAnimationSet;
    configInfo.size = sets.size;
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
    appRestart();
}

// ==============================================================
// 设置自启动
// ==============================================================
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
// ==============================================================
// 设置动画
// 当动画功能设置为关闭时，
// 如果当前时钟状态为隐藏或者正在移动到隐藏状态，则启动动画从隐藏到显示
// 如果当前时钟状态为显示或者正在移动到显示状态，则不做任何操作
// ==============================================================
void TrayMenu::SetAnimation()
{
    logger->info("[SetAnimation] set animation {}", configInfo.isAnimationSet ? "true" : "false");
    if (!configInfo.isAnimationSet)
    {
        if (GetClockBodyCurState() == CLOCKBODY_HIDE || GetClockBodyCurState() == CLOCKBODY_MOVE_TO_HIDE)
        {
            SetClockBodyCurState(ClockBodyState::CLOCKBODY_MOVE_TO_SHOW);
            animationHideToShow(StateConditionGuard::GUARD_CLOSE);
        }
    }
}

// ==============================================================
// 初始化时间列表和结束时间列表
// ==============================================================
void TrayMenu::initTime()
{
    logger->info("[initTime] init time");
    if (configInfo.isAnimationSet)
    {
        timeStringToTimeList(configInfo.secondList, SecondList, 60);
        timeStringToTimeList(configInfo.minuteList, MinuteList, 60);
        timeStringToTimeList(configInfo.hourList, HourList, 24);
        timeStringToTimeList(configInfo.dayList, WeekList, 7);

        SecondCloseList->clear();
        MinuteCloseList->clear();
        HourCloseList->clear();
        WeekCloseList->clear();

        for (auto dayit = WeekList->begin(); dayit != WeekList->end(); dayit++)
        {
            for (auto hourit = HourList->begin(); hourit != HourList->end(); hourit++)
            {
                for (auto minuteit = MinuteList->begin(); minuteit != MinuteList->end(); minuteit++)
                {
                    for (auto secondit = SecondList->begin(); secondit != SecondList->end(); secondit++)
                    {
                        int advanceFlag = 0;
                        int finalSecond = -1;
                        int finalMinute = -1;
                        int finalHour = -1;
                        int finalDay = -1;

                        auto advanceCharge = [](int num, int advance) -> int {
                            return num >= advance? 1 : 0;
                        };

                        auto advanceRemove = [](int num, int advance) -> int {
                            return num % advance;
                        };
                        auto numberTrans = [](int num) -> QString {
                            return num == -1 ? "xx" : (num > 9 ? QString::number(num) : ("0" + QString::number(num)));
                        };

                        if( *secondit != "xx" ){
                            finalSecond = (*secondit).toInt() + configInfo.clockMoveInterval;
                            advanceFlag = advanceCharge(finalSecond, 60);
                            finalSecond = advanceRemove(finalSecond, 60);
                        }
                        if( *minuteit != "xx" ){
                            finalMinute = (*minuteit).toInt() + advanceFlag;
                            advanceFlag = advanceCharge(finalMinute, 60);
                            finalMinute = advanceRemove(finalMinute, 60);
                        }
                        if( *hourit != "xx" ){
                            finalHour = (*hourit).toInt() + advanceFlag;
                            advanceFlag = advanceCharge(finalHour, 24);
                            finalHour = advanceRemove(finalHour, 24);
                        }

                        if( *dayit != "xx" ){
                            finalDay = (*dayit).toInt() + advanceFlag;
                            advanceFlag = advanceCharge(finalDay, 7);
                            finalDay = advanceRemove(finalDay, 7);
                        }

                        logger->info("[initTime] startTime: {} {}:{}:{}", findWeekDayName(finalDay).toStdString(), (*hourit).toStdString(), (*minuteit).toStdString(), (*secondit).toStdString());
                        QString finalTime = findWeekDayName(finalDay) + " " + numberTrans(finalHour) + ":" + numberTrans(finalMinute) + ":" + numberTrans(finalSecond);
                        logger->info("[initTime] finalTime: " + finalTime.toStdString());
                        SecondCloseList->append(numberTrans(finalSecond));
                        MinuteCloseList->append(numberTrans(finalMinute));
                        HourCloseList->append(numberTrans(finalHour));
                        WeekCloseList->append(numberTrans(finalDay));
                    }
                }
            }
        }
    }
}

// ==============================================================
// 将时间字符串转换为时间列表
// 功能：1. 解析时间字符串，将时间字符串转换为时间列表 2. 如果时间字符串为空，则初始化时间列表为默认值 xx
// ==============================================================
void TrayMenu::timeStringToTimeList(QString timeString, QList<QString> *timeList, const int &maxNum)
{
    timeList->clear();
    if (timeString.isEmpty()) // 如果时间字符串为空，则初始化时间列表为默认值 xx, 且不重复保存
    {
        for (size_t i = 0; i < maxNum; i++)
        {
            if (7 == maxNum) // 如果是星期列表，则将数字转换为星期名称
            {
                timeList->append(QString::number(i));
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
            timeList->append(*i);
        }
    }
}

// ==============================================================
// 检查时间是否在时间列表中
// ==============================================================
void TrayMenu::CheckTimeToCtrlClock()
{
    if (configInfo.isAnimationSet)
    {
        logger->debug("show Curstate = {}", GetClockBodyCurState());
        QDateTime current_date_time = QDateTime::currentDateTime();
        int curtime = current_date_time.toSecsSinceEpoch();
        QString current_week = current_date_time.toString("dddd");
        QString current_hour = current_date_time.toString("hh");
        QString current_minute = current_date_time.toString("mm");
        QString current_second = current_date_time.toString("ss");

        if (GetClockBodyCurState() == ClockBodyState::CLOCKBODY_HIDE || GetClockBodyCurState() == ClockBodyState::CLOCKBODY_MOVE_TO_HIDE)
        {
            for (auto k = WeekList->begin(); k != WeekList->end(); k++)
            {
                logger->debug("current_week = {}, find = {}", current_week.toStdString(), findWeekDayName((*k).toInt()).toStdString());
                if (current_week == findWeekDayName((*k).toInt()))
                {
                    for (auto j = HourList->begin(); j != HourList->end(); j++)
                    {
                        if (current_hour == *j || "xx" == *j)
                        {
                            for (auto l = MinuteList->begin(); l != MinuteList->end(); l++)
                            {
                                if (current_minute == *l || "xx" == *l)
                                {
                                    for (auto i = SecondList->begin(); i != SecondList->end(); i++)
                                    {
                                        if (current_second == *i || "xx" == *i)
                                        {
                                            logger->debug("time to move clock = {}", current_date_time.toString("dddd hh:mm:ss").toStdString());
                                            SetClockBodyCurState(ClockBodyState::CLOCKBODY_MOVE_TO_SHOW);
                                            animationHideToShow(StateConditionGuard::GUARD_OPEN);
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (GetClockBodyCurState() == ClockBodyState::CLOCKBODY_SHOW || GetClockBodyCurState() == ClockBodyState::CLOCKBODY_MOVE_TO_SHOW)
        {
            for (auto k = WeekCloseList->begin(); k != WeekCloseList->end(); k++)
            {
                if (current_week == findWeekDayName((*k).toInt()))
                {
                    for (auto j = HourCloseList->begin(); j != HourCloseList->end(); j++)
                    {
                        if (current_hour == *j || "xx" == *j)
                        {
                            for (auto l = MinuteCloseList->begin(); l != MinuteCloseList->end(); l++)
                            {
                                if (current_minute == *l || "xx" == *l)
                                {
                                    for (auto i = SecondCloseList->begin(); i != SecondCloseList->end(); i++)
                                    {
                                        if (current_second == *i || "xx" == *i)
                                        {
                                            logger->info("time to back clock = {}", current_date_time.toString("dddd hh:mm:ss").toStdString());
                                            SetClockBodyCurState(ClockBodyState::CLOCKBODY_MOVE_TO_HIDE);
                                            animationShowToHide(StateConditionGuard::GUARD_OPEN);
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void TrayMenu::SetClockBodyCurState(const ClockBodyState &state)
{
    m_clockBodyStateMachine.currentState = state;
    m_clockBodyStateMachine.targetState = static_cast<ClockBodyState>((state + 1) % sizeof(ClockBodyState));
    logger->info("state changed to {}", state);
}

const ClockBodyState TrayMenu::GetClockBodyCurState()
{
    return m_clockBodyStateMachine.currentState;
}

const ClockBodyState TrayMenu::GetClockBodyTargetState()
{
    return m_clockBodyStateMachine.targetState;
}
