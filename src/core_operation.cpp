#include "core_operation.h"
#pragma execution_character_set("utf-8")

PopupClock::PopupClock(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    // lcd时间初始化
    ui.lcdNumber->display(QTime::currentTime().toString("hh:mm:ss"));
    logger->info("lcd number display INIT");
    // 设置定时器
    logger->info("lcd number timer INIT");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(SetNumClock()));
    timer->start(1000);

    // 设置窗口背景透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    logger->info("set window TranslucentBackground");

    // 设置窗口置顶
    this->TopSetWindow();
}

void PopupClock::SetClockStatus(int px, int py, bool active, int speed, int ktime, QList<QString> *S, QList<QString> *M, QList<QString> *H, QList<QString> *W)
{
    AnimateCtrl(active);
    moveSpeed = speed;
    keepTime = ktime;
    if (SecondList != nullptr)
    {
        delete SecondList;
        delete MinuteList;
        delete HourList;
        delete WeekList;
    }
    SecondList = S;
    MinuteList = M;
    HourList = H;
    WeekList = W;
    m_x = px;
    m_y = py;
    logger->info("SetClockStatus Pos({}, {}), MoveSpeed({}), KeepTime({})", px, py, speed, ktime);
}

void PopupClock::AnimateCtrl(bool active)
{
    animateActive = active;
    logger->info("Set AnimateCtrl [bool] {}", active);
}

void PopupClock::TopSetWindow()
{
#ifdef Q_OS_WIN
    ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#else
    // other
#endif
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    logger->info("show window");
    show();
    activateWindow();
    logger->info("set window on the top layer");
}

void PopupClock::SetNumClock()
{
    // logger->info("[Number Timer] reset lcd number display");
    ui.lcdNumber->display(QTime::currentTime().toString("hh:mm:ss"));

    if (animateActive && WeekList != nullptr)
    {
        // logger->info("[Number Timer] animateActive {} , check time list", animateActive);
        QDateTime current_date_time = QDateTime::currentDateTime();
        int curtime = current_date_time.toSecsSinceEpoch();
        QString current_week = current_date_time.toString("dddd");
        QString current_hour = current_date_time.toString("hh");
        QString current_minute = current_date_time.toString("mm");
        QString current_second = current_date_time.toString("ss");

        for (auto k = WeekList->begin(); k != WeekList->end(); k++)
        {

            if (current_week != findWeekDayName((*k).toInt()))
                continue;
            for (auto j = HourList->begin(); j != HourList->end(); j++)
            {

                if (current_hour != *j)
                    continue;

                for (auto l = MinuteList->begin(); l != MinuteList->end(); l++)
                {
                    if (current_minute != *l)
                        continue;
                    for (auto i = SecondList->begin(); i != SecondList->end(); i++)
                    {

                        if (current_second == *i)
                        {
                            logger->info("[Number Timer] Move start time {} {}:{}:{} , keepTime {}", std::string(current_week.toLocal8Bit()), std::string(current_hour.toLocal8Bit()), std::string(current_minute.toLocal8Bit()), std::string(current_second.toLocal8Bit()), keepTime);
                            MoveClock();
                            finishTime = curtime + keepTime;
                            break;
                        }
                    }
                }
            }
        }
        if (curtime == finishTime)
        {
            logger->info("[Number Timer] Move finish time {} {}:{}:{} , keepTime {}", std::string(current_week.toLocal8Bit()), std::string(current_hour.toLocal8Bit()), std::string(current_minute.toLocal8Bit()), std::string(current_second.toLocal8Bit()), keepTime);
            MoveClockback();
        }
    }
}

void PopupClock::MoveClock()
{
    isPop = true;
    logger->info("[ClockMove Action] start pop up, isPop({})", isPop);
    logger->info("[ClockMove Action] Animation Start, speed({}), from({}, {}) to ({}, {})", moveSpeed, m_x - moveStep, m_y, m_x, m_y);
    QPropertyAnimation *m_pAnimation = &m_Animation0;
    m_pAnimation->setTargetObject(this);
    m_pAnimation->setPropertyName("pos");
    m_pAnimation->setDuration(moveSpeed);
    m_pAnimation->setStartValue(QPoint(m_x - moveStep, m_y));
    m_pAnimation->setEndValue(QPoint(m_x, m_y));
    m_pAnimation->start();

    QPropertyAnimation *m_pAnimation2 = &m_Animation1;
    m_pAnimation2->setTargetObject(this);
    m_pAnimation2->setPropertyName("windowOpacity");
    m_pAnimation2->setDuration(moveSpeed);
    m_pAnimation2->setKeyValueAt(0, 0);
    m_pAnimation2->setKeyValueAt(0.5, 1);
    m_pAnimation2->setKeyValueAt(1, 1);
    m_pAnimation2->start();
    logger->info("[ClockMove Action] Animation Finish");
}

void PopupClock::MoveClockback()
{
    isPop = false;
    logger->info("[ClockMove Action] start push back, isPop({})", isPop);
    logger->info("[ClockMove Action] Animation Start, speed({}), from({}, {}) to ({}, {})", moveSpeed, m_x, m_y, m_x - moveStep, m_y);
    QPropertyAnimation *m_pAnimation = &m_Animation0;
    m_pAnimation->setTargetObject(this);
    m_pAnimation->setPropertyName("pos");
    m_pAnimation->setDuration(moveSpeed);
    m_pAnimation->setStartValue(QPoint(m_x, m_y));
    m_pAnimation->setEndValue(QPoint(m_x - moveStep, m_y));
    m_pAnimation->start();
    QPropertyAnimation *m_pAnimation2 = &m_Animation1;
    m_pAnimation2->setTargetObject(this);
    m_pAnimation2->setPropertyName("windowOpacity");
    m_pAnimation2->setDuration(moveSpeed);
    m_pAnimation2->setKeyValueAt(0, 1);
    m_pAnimation2->setKeyValueAt(0.5, 1);
    m_pAnimation2->setKeyValueAt(1, 0);
    m_pAnimation2->start();
    logger->info("[ClockMove Action] Animation Finish");
}
void PopupClock::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        tempState = animateActive;
        animateActive = false;
        last_mouse_position_ = event->globalPos();
        logger->info("[Mouse Event] LeftButton Down, pos({}, {}), isPop({})", last_mouse_position_.x(), last_mouse_position_.y(), isPop);
    }else{
        logger->warn("[Mouse Event] event->button() : {}", event->button());
    }
}

void PopupClock::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::LeftButton))
        return;
    const QPoint position = pos() + event->globalPos() - last_mouse_position_;
    move(position.x(), position.y());
    last_mouse_position_ = event->globalPos();
}

void PopupClock::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) // Left button...
    {
        logger->info("[Mouse Event] LeftButton Up, pos({}, {}), isPop({})", last_mouse_position_.x(), last_mouse_position_.y(), isPop);
        animateActive = tempState;
        if (isPop)
        {
            m_x = this->pos().x();
            m_y = this->pos().y();
        }
        else
        {
            m_x = this->pos().x() + moveStep;
            m_y = this->pos().y();
        }
        auto config = new QSettings(filePath, QSettings::IniFormat);
        config->setValue("Config/ClockX", m_x);
        config->setValue("Config/ClockY", m_y);
        logger->info("[Config Event] Save Pos({}, {})", m_x, m_y);
        config->sync();
    }
}

bool PopupClock::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this && event->type() == QEvent::WindowDeactivate)
    {
        logger->info("[Window Event] WindowDeactivate");
        activateWindow();
    }
    return true;
}

void PopupClock::ShowClock()
{
    if (!isPop)
    {
        if (animateActive)
        {
            logger->info("[Window Event] ShowClock");
            MoveClock();
        }
    }
}

void PopupClock::HideClock()
{
    if (!isPop)
    {
        if (animateActive)
        {
            logger->info("[Window Event] HideClock");
            MoveClockback();
        }
    }
}

QString PopupClock::findWeekDayName(int index)
{
    return Weeks[index];
}
