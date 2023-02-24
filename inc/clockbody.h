#ifndef CLOCKBODY_H
#define CLOCKBODY_H

#include "common.h"
extern std::shared_ptr<spdlog::logger> logger;

namespace
{
    struct ClockBodyStruct
    {
        int width;
        int height;
        QColor backgroundColor;
        int borderRadius;
    };
    struct ClockDialStruct
    {
        int width;
        int height;
        QColor backgroundColor;
        QColor secondHandColor;
        QColor minuteHandColor;
        QColor hourHandColor;
        int positionX;
        int positionY;
    };
    struct ClockNumberStruct
    {
        int width;
        int height;
        QColor backgroundColor;
        int borderRadius;
        int positionX;
        int positionY;

    
    };

    const QPoint hourHand[4] = {
        QPoint(4, 0),
        QPoint(-4, 0),
        QPoint(-4, -50),
        QPoint(4, -50)};
    const QPoint minuteHand[4] = {
        QPoint(3, 0),
        QPoint(-3, 0),
        QPoint(-3, -80),
        QPoint(3, -80)};
    const QPoint secondHand[4] = {
        QPoint(1, 0),
        QPoint(-1, 0),
        QPoint(-1, -100),
        QPoint(1, -100)};
} // namespace

class ClockBody : public QWidget
{
    Q_OBJECT
public:
    ClockBody(QWidget *parent = nullptr);
    ~ClockBody();

    void initWidget();
    void drawClockBody();
    void drawClockDial();
    void drawClockNumber();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


    void drawHourHand(QPainter *painter, QTime time);
    void drawMinuteHand(QPainter *painter, QTime time);
    void drawsecondHand(QPainter *painter, QTime time);
    void drawBackgroud(QPainter *painter);
    void drawCentre(QPainter *painter);



signals:

private:
    int tClockBodyWidth = 280;
    int tClockBodyHeight = 90;
    int tClockDialWidth = 60;
    int tClockDialHeight = tClockDialWidth;
    int tClockDialPositionY = (tClockBodyHeight - tClockDialHeight) / 2;
    int tClockDialPositionX = 10;
    int tClockNumberWidth = 190;
    int tClockNumberHeight = tClockDialHeight;
    int tClockNumberPositionX = (tClockBodyWidth - tClockNumberWidth - tClockDialPositionX - tClockDialWidth) / 2 + tClockDialPositionX + tClockDialWidth;
    int tClockNumberPositionY = tClockDialPositionY;


    ClockBodyStruct clockBodyInfo{
        tClockBodyWidth,
        tClockBodyHeight,
        QColor(189, 189, 189),
        18};
    ClockDialStruct clockDialInfo{
        tClockDialWidth,
        tClockDialHeight,
        QColor(230, 230, 230),
        Qt::red,
        Qt::black,
        Qt::black,
        tClockDialPositionX,
        tClockDialPositionY
    };
    ClockNumberStruct clockNumberInfo{
        tClockNumberWidth,
        tClockNumberHeight,
        QColor(230, 230, 230),
        18,
        tClockNumberPositionX,
        tClockNumberPositionY
    };

    QPoint dragPosition;
    QLCDNumber *lcd = new QLCDNumber(this);
};

#endif // CLOCKBODY_H
