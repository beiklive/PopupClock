#pragma once

#include <QtWidgets/QWidget>
#include "ui_PopupClock.h"
#include <QTimer>
#include <QMouseEvent>
#include <QTime>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include "DrawClock.h"
#include <QPropertyAnimation>
class PopupClock : public QWidget
{
    Q_OBJECT

public:
    PopupClock(QWidget *parent = Q_NULLPTR);

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
private:
    Ui::PopupClockClass ui;
	QPoint last_mouse_position_;
	QSystemTrayIcon* mSysTrayIcon;
	int m_x = 250;
	int chargeMin = 30;
	int chargeSec = 30;

public slots:
	void SetNumClock();
	void MoveClock();
	void MoveClockback();
};
