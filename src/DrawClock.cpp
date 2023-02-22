#include "DrawClock.h"

DrawClock::DrawClock(QWidget *parent)
	: QWidget(parent)
{
	QTimer* timer = new QTimer(this);
	timer->start(500);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

const QPoint DrawClock::hourHand[4] = {
	QPoint(4, 0),
	QPoint(-4, 0),
	QPoint(-4, -50),
	QPoint(4, -50)
};
const QPoint DrawClock::minuteHand[4] = {
	QPoint(3, 0),
	QPoint(-3, 0),
	QPoint(-3, -80),
	QPoint(3, -80)
};
const QPoint DrawClock::secondHand[4] = {
	QPoint(1, 0),
	QPoint(-1, 0),
	QPoint(-1, -100),
	QPoint(1, -100)
};


void DrawClock::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.translate(width() / 2, height() / 2);
	int size = width() < height() ? width() : height();
	painter.scale(size / 220.0, size / 220.0);
    time = QTime::currentTime();
	drawBackgroud(&painter);
	drawHourHand(&painter);
	drawMinuteHand(&painter);
	drawsecondHand(&painter);
	painter.setBrush(Qt::black);
	drawCentre(&painter);
}
void DrawClock::drawHourHand(QPainter* painter)
{
	painter->setBrush(Qt::black);
	painter->setPen(Qt::black);
	painter->save();
	painter->rotate(30.0 * (time.hour() + time.minute() / 60.0) + time.second() / 3600.0);
	painter->drawConvexPolygon(hourHand, 4);
	painter->restore();
}
void DrawClock::drawMinuteHand(QPainter* painter)
{
	painter->setBrush(Qt::black);
	painter->setPen(Qt::black);
	painter->save();
	painter->rotate(6.0 * (time.minute() + time.second() / 60.0));
	painter->drawConvexPolygon(minuteHand, 4);
	painter->restore();
}
void DrawClock::drawsecondHand(QPainter* painter)
{
	painter->setBrush(Qt::red);
	painter->setPen(Qt::red);
	painter->save();
	painter->rotate(6.0 * time.second());
	painter->drawConvexPolygon(secondHand, 4);
	painter->restore();
}

void DrawClock::drawBackgroud(QPainter* painter)
{
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);;
	QRadialGradient radialGradient(QPointF(0, 0), 110, QPointF(0, 0));
	radialGradient.setColorAt(1, QColor(230, 230, 230, 255));
	radialGradient.setColorAt(0.9, QColor(230, 230, 230, 255));
	radialGradient.setColorAt(0.89, QColor(230, 230, 230, 255));
	painter->setPen(Qt::NoPen);
	painter->setBrush(radialGradient);
	painter->drawEllipse(QPointF(0, 0), 110, 110);
}

void DrawClock::drawCentre(QPainter* painter)
{
	QConicalGradient coneGradient(0, 0, -90.0);
	coneGradient.setColorAt(0.0, Qt::darkGray);
	coneGradient.setColorAt(0.2, QColor(150, 150, 200));
	coneGradient.setColorAt(0.5, Qt::white);
	coneGradient.setColorAt(1.0, Qt::darkGray);
	painter->setPen(Qt::NoPen);
	painter->setBrush(coneGradient);
	painter->drawEllipse(-5, -5, 10, 10);
}
