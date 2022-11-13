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
#include <QFileInfo>
#include <QSettings>
#include <QDir>
class PopupClock : public QWidget
{
    Q_OBJECT

public:
    PopupClock(QWidget *parent = Q_NULLPTR);

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

private:
	bool checkAutoStart();
private:
    Ui::PopupClockClass ui;
	QPoint last_mouse_position_;
	QSystemTrayIcon* mSysTrayIcon;
	int m_x = 250;
	int chargeMin1 = 29;
	int chargeMin2 = 59;
	int chargeSec = 30;

	QAction* m_pActionAutoStart;
	QAction* m_pActionExit;

public slots:
	void SetNumClock();
	void MoveClock();
	void MoveClockback();
	void SetAutoStart(bool flag);
};
