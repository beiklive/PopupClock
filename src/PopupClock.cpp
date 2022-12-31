#include "PopupClock.h"
#include <QSystemTrayIcon>
#include <QIcon>
#include <QDebug>
#pragma execution_character_set("utf-8")






PopupClock::PopupClock(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

	QTime time = QTime::currentTime();
	QString txtTime = time.toString("hh:mm:ss");
	ui.lcdNumber->display(txtTime);
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(SetNumClock()));
    timer->start(1000);
}

void PopupClock::SetClockStatus(int px, int py, bool active, int speed, int ktime, QList<QString> *S, QList<QString> *M, QList<QString> *H, QList<QString> *W)
{
    AnimateCtrl(active);
    moveSpeed = speed;
    keepTime = ktime;
    if(SecondList != nullptr)
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

}

void PopupClock::AnimateCtrl(bool active)
{
    animateActive = active;
}

void PopupClock::SetNumClock()
{
    QTime time = QTime::currentTime();
    QString txtTime = time.toString("hh:mm:ss");

    ui.lcdNumber->display(txtTime);


    if(animateActive && WeekList != nullptr){
        QDateTime current_date_time = QDateTime::currentDateTime();
        int curtime = current_date_time.toSecsSinceEpoch();
        QString current_week = current_date_time.toString("dddd");
        QString current_hour = current_date_time.toString("hh");
        QString current_minute = current_date_time.toString("mm");
        QString current_second = current_date_time.toString("ss");

        for (auto k = WeekList->begin(); k != WeekList->end(); k++)
        {

            if(current_week != findWeekDayName((*k).toInt()))
                continue;
            for (auto j = HourList->begin(); j != HourList->end(); j++)
            {

                if(current_hour != *j)
                    continue;

                for (auto l = MinuteList->begin(); l != MinuteList->end(); l++)
                {
                    if(current_minute != *l)
                        continue;
                    for (auto i = SecondList->begin(); i != SecondList->end(); i++)
                    {

                        if(current_second == *i)
                        {
                            MoveClock();
                            finishTime = curtime + keepTime;
                            break;
                        }

                    }
                }
            }
        }
        if(curtime == finishTime){
            MoveClockback();
        }
    }

}





void PopupClock::MoveClock() {
    isPop = true;

    QPropertyAnimation* m_pAnimation =&m_Animation0;
    m_pAnimation->setTargetObject(this);
    m_pAnimation->setPropertyName("pos");
    m_pAnimation->setDuration(moveSpeed);
    m_pAnimation->setStartValue(QPoint(m_x - moveStep, m_y));
    m_pAnimation->setEndValue(QPoint(m_x, m_y));
	m_pAnimation->start();

    QPropertyAnimation* m_pAnimation2 = &m_Animation1;
    m_pAnimation2->setTargetObject(this);
    m_pAnimation2->setPropertyName("windowOpacity"); 
    m_pAnimation2->setDuration(moveSpeed);
    m_pAnimation2->setKeyValueAt(0, 0);
    m_pAnimation2->setKeyValueAt(0.5, 1);
    m_pAnimation2->setKeyValueAt(1, 1);
    m_pAnimation2->start();
}

void PopupClock::MoveClockback() {
    isPop = false;
    QPropertyAnimation* m_pAnimation =&m_Animation0;
    m_pAnimation->setTargetObject(this);
    m_pAnimation->setPropertyName("pos");
    m_pAnimation->setDuration(moveSpeed);
    m_pAnimation->setStartValue(QPoint(m_x, m_y));
    m_pAnimation->setEndValue(QPoint(m_x - moveStep, m_y));
	m_pAnimation->start();
    QPropertyAnimation* m_pAnimation2 = &m_Animation1;
    m_pAnimation2->setTargetObject(this);
    m_pAnimation2->setPropertyName("windowOpacity");
    m_pAnimation2->setDuration(moveSpeed); 
    m_pAnimation2->setKeyValueAt(0, 1);
    m_pAnimation2->setKeyValueAt(0.5, 1);
    m_pAnimation2->setKeyValueAt(1, 0);
    m_pAnimation2->start();

}
void PopupClock::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton){
        tempState = animateActive;
        animateActive = false;
		last_mouse_position_ = event->globalPos();
    }
}

void PopupClock::mouseMoveEvent(QMouseEvent* event)
{
	if (!event->buttons().testFlag(Qt::LeftButton))
		return;
	const QPoint position = pos() + event->globalPos() - last_mouse_position_;
	move(position.x(), position.y());
	last_mouse_position_ = event->globalPos();

}

void PopupClock::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)    // Left button...
    {
        animateActive = tempState;
        if(isPop){
            m_x = this->pos().x();
            m_y = this->pos().y();
        }else{
            m_x = this->pos().x() + moveStep;
            m_y = this->pos().y();
        }
        auto config = new QSettings(filePath,QSettings::IniFormat);
        config->setValue("Config/ClockX",m_x);
        config->setValue("Config/ClockY",m_y);
        config->sync();
    }
}

void PopupClock::ShowClock()
{
    if(!isPop){
        if(animateActive){
            MoveClock();
        }
    }
}

void PopupClock::HideClock()
{
    if(!isPop){
        if(animateActive){
            MoveClockback();
        }
    }
}

QString PopupClock::findWeekDayName(int index)
{
    return Weeks[index];
}
