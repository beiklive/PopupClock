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
	static const QPoint hourHand[4];//小时指针
	static const QPoint minuteHand[4];//分钟指针
	static const QPoint secondHand[4];//秒指针

protected:
	void paintEvent(QPaintEvent*);//窗体重绘事件
	void drawHourHand(QPainter* painter);//画 时针
	void drawMinuteHand(QPainter* painter);//画 分针
	void drawsecondHand(QPainter* painter);//画 秒针
	void drawBackgroud(QPainter* painter);//画 背景
	void drawCentre(QPainter* painter);//画 中心点
};
