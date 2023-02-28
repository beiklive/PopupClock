#ifndef CLOCKBODY_H
#define CLOCKBODY_H

#include "common.h"
extern std::shared_ptr<spdlog::logger> logger;
namespace
{
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
    void drawClockBody(const QTime& time);
    void drawClockDial(const QTime& time);
    void drawClockNumber(const QTime& time);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


    void drawHourHand(QPainter *painter, const QTime& time);
    void drawMinuteHand(QPainter *painter, const QTime& time);
    void drawsecondHand(QPainter *painter, const QTime& time);
    void drawBackgroud(QPainter *painter);
    void drawCentre(QPainter *painter);

signals:
    void mouseReleaseSignal(QPoint pos);
    void mousePressSignal();

private:
    int mSizeStep = 10;
    int tClockBodyWidth = 28 * mSizeStep;
    int tClockBodyHeight = 9 * mSizeStep;
    int tClockDialWidth = 6 * mSizeStep;
    int tClockDialHeight = tClockDialWidth;
    int tClockDialPositionY = (tClockBodyHeight - tClockDialHeight) / 2;
    int tClockDialPositionX = 1 * mSizeStep;
    int tClockNumberWidth = 19 * mSizeStep;
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
