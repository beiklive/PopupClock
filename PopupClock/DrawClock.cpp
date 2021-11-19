#include "DrawClock.h"

DrawClock::DrawClock(QWidget *parent)
	: QWidget(parent)
{
	//创建一个定时器
	QTimer* timer = new QTimer(this);
	//开启定时器，设置为1秒
	timer->start(500);
	//定时器溢出时，执行相应动作，这里是每一秒进行更新窗体，进行窗体重绘
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

DrawClock::~DrawClock()
{
}

const QPoint DrawClock::hourHand[4] = {               //四边形时针图形的四个角的坐标点
	QPoint(4, 0),//中心点、右侧
	QPoint(-4, 0),//针头
	QPoint(-4, -50),//中心点、左侧
	QPoint(4, -50)//针尾    (注意映射后坐标原点是(110,110))
};
const QPoint DrawClock::minuteHand[4] = {
	QPoint(3, 0),
	QPoint(-3, 0),
	QPoint(-3, -80),
	QPoint(3, -80)
};
const QPoint DrawClock::secondHand[4] = {
	QPoint(1, 0),//右侧
	QPoint(-1, 0),//左侧
	QPoint(-1, -100),//针尾
	QPoint(1, -100)//针尾
};


void DrawClock::paintEvent(QPaintEvent*)
{
	//将当前窗体作为画布
	QPainter painter(this);
	//消除锯齿，看起来更加圆滑
	painter.setRenderHint(QPainter::Antialiasing, true);
	// 将绘画原点设置为为窗体中心
	painter.translate(width() / 2, height() / 2);
	//获取 窗体的宽、窗体的高 两个值的较小值
	int size = width() < height() ? width() : height();
	//按照比例进行缩放
	painter.scale(size / 220.0, size / 220.0);
	//画背景
	drawBackgroud(&painter);
	//画时针
	drawHourHand(&painter);
	//画分针
	drawMinuteHand(&painter);
	//画秒针
	drawsecondHand(&painter);
	//设置画刷
	painter.setBrush(Qt::black);
	//画中心点
	drawCentre(&painter);
}
void DrawClock::drawHourHand(QPainter* painter)
{
	//获取当前时间
	QTime time = QTime::currentTime();
	//设置画刷
	painter->setBrush(Qt::black);
	//设置画笔
	painter->setPen(Qt::black);
	//保存当前painter状态到栈中、因为后面旋转后要恢复
	painter->save();
	//旋转角度  1个小时30°(5个刻度)     当前分钟的小时数   当前秒的小时数
	painter->rotate(30.0 * (time.hour() + time.minute() / 60.0) + time.second() / 3600.0);
	//绘制时针  (绘制多边形多边形)
	painter->drawConvexPolygon(hourHand, 4);
	//绘制图形后恢复之前painter状态(出栈)
	painter->restore();
}
void DrawClock::drawMinuteHand(QPainter* painter)
{
	//获取当前时间
	QTime time = QTime::currentTime();
	//设置画刷
	painter->setBrush(Qt::black);
	//设置画笔
	painter->setPen(Qt::black);
	//保存当前painter状态到栈中、因为后面旋转后要恢复
	painter->save();
	//旋转角度     1分钟1刻度(6°)         当前秒数的小时数
	painter->rotate(6.0 * (time.minute() + time.second() / 60.0));
	//绘制分针
	painter->drawConvexPolygon(minuteHand, 4);
	//绘制图形后恢复之前状态(出栈)
	painter->restore();
}
void DrawClock::drawsecondHand(QPainter* painter)
{
	//获取当前时间
	QTime time = QTime::currentTime();
	//设置画刷
	painter->setBrush(Qt::red);
	//设置画笔
	painter->setPen(Qt::red);
	//保存当前painter状态到栈中、因为后面旋转后要恢复
	painter->save();
	//旋转
	painter->rotate(6.0 * time.second());
	//绘制秒针
	painter->drawConvexPolygon(secondHand, 4);
	//绘制图形后恢复之前painter状态(出栈)
	painter->restore();
}

void DrawClock::drawBackgroud(QPainter* painter)
{
	//消除锯齿，看起来更加圆滑
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);;
	//辐射渐变(从圆的边渐变到焦点)                    圆心         半径        焦点
	QRadialGradient radialGradient(QPointF(0, 0), 110, QPointF(0, 0));
	//从圆的边渐变到焦点，刻度从1~0，下面例子：在1~0.9刻度从颜色1渐变到颜色2，在0.9~0.89刻度从颜色2渐变到颜色3
	radialGradient.setColorAt(1, QColor(230, 230, 230, 255));//颜色1
	radialGradient.setColorAt(0.9, QColor(230, 230, 230, 255));//颜色2
	radialGradient.setColorAt(0.89, QColor(230, 230, 230, 255));//颜色3
	//边框线无色
	painter->setPen(Qt::NoPen);
	//使用辐射渐变作为画刷
	painter->setBrush(radialGradient);
	//画一个圆                椭圆中心       短轴 长轴
	painter->drawEllipse(QPointF(0, 0), 110, 110);
}

void DrawClock::drawCentre(QPainter* painter)
{
	//画中心点
	//角度渐变
	QConicalGradient coneGradient(0, 0, -90.0);
	//渐变颜色、渐变位置设置
	coneGradient.setColorAt(0.0, Qt::darkGray);
	coneGradient.setColorAt(0.2, QColor(150, 150, 200));
	coneGradient.setColorAt(0.5, Qt::white);
	coneGradient.setColorAt(1.0, Qt::darkGray);
	//没有线，填满没有边界
	painter->setPen(Qt::NoPen);
	//设置画刷
	painter->setBrush(coneGradient);
	//绘制椭圆
	painter->drawEllipse(-5, -5, 10, 10);
}