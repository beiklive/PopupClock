#pragma once

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include "core_logger.hpp"
extern std::shared_ptr<spdlog::logger> logger;
class DrawClock : public QWidget
{
    Q_OBJECT

public:
    DrawClock(QWidget *parent);
    ~DrawClock() = default;

private:
    static const QPoint hourHand[4];
    static const QPoint minuteHand[4];
    static const QPoint secondHand[4];
    const int paint_interval_ms{500};
    QTime time;

protected:
    void paintEvent(QPaintEvent *);
    void drawHourHand(QPainter *painter);
    void drawMinuteHand(QPainter *painter);
    void drawsecondHand(QPainter *painter);
    void drawBackgroud(QPainter *painter);
    void drawCentre(QPainter *painter);
};
