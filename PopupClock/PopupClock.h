#pragma once

#include <QtWidgets/QWidget>
#include "ui_PopupClock.h"

#include <QTimer>
#include <QMouseEvent>
#include <QTime>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QList>
class PopupClock : public QWidget
{
    Q_OBJECT

public:
    PopupClock(QWidget *parent = Q_NULLPTR);
    void SetClockStatus(bool active, int speed, int ktime, QList<QString> *S, QList<QString> *M, QList<QString> *H, QList<QString> *W);
    void AnimateCtrl(bool active);
    void DoDisplay();
protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);


private:
    const QString Weeks[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    QString findWeekDayName(int index);

    Ui::PopupClockClass ui;
//    Sets settingUI;
	QPoint last_mouse_position_;
//	QSystemTrayIcon* mSysTrayIcon;
// options
    int moveStep = 350;
    int m_x = 550;
    int m_y = 50;
    int moveSpeed = 1500;
    int keepTime = 60;
    int finishTime = 0;
    bool isPop = true;
    bool tempState;

    QList<QString> *SecondList{nullptr};
    QList<QString> *MinuteList{nullptr};
    QList<QString> *HourList{nullptr};
    QList<QString> *WeekList{nullptr};

    bool animateActive = false;


public slots:
	void SetNumClock();
	void MoveClock();
	void MoveClockback();


};
