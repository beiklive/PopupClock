#pragma once

#include <QWidget>

#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
class DrawClock : public QWidget
{
	Q_OBJECT

public:
	DrawClock(QWidget *parent);
	~DrawClock();
private:
	static const QPoint hourHand[4];
	static const QPoint minuteHand[4];
	static const QPoint secondHand[4];

protected:
	void paintEvent(QPaintEvent*);
	void drawHourHand(QPainter* painter);
	void drawMinuteHand(QPainter* painter);
	void drawsecondHand(QPainter* painter);
	void drawBackgroud(QPainter* painter);
	void drawCentre(QPainter* painter);
};
