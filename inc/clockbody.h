#ifndef CLOCKBODY_H
#define CLOCKBODY_H

#include "common.h"
extern std::shared_ptr<spdlog::logger> logger;
extern ConfigManager& Config;
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
    void initLayout(int value);
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

public slots:
    void bodySizeSlot(int value);
signals:
    void mouseReleaseSignal(QPoint pos);
    void mousePressSignal();

private:
    int mSizeStep = 3;
    int gap = 8;
    int fontGapX = 8;
    int fontGapY = 8;
    int tClockNumberFontPosX;
    int tClockNumberFontPosY;
    int tClockNumberFontWidth;
    int tClockNumberFontHeight;
    ClockBodyStruct clockBodyInfo{
        80,
        80,
        QColor(189, 189, 189),
        80};
    ClockDialStruct clockDialInfo{
        80,
        80,
        QColor(230, 230, 230),
        Qt::red,
        Qt::black,
        Qt::black,
        80,
        80
    };
    ClockNumberStruct clockNumberInfo{
        80,
        80,
        QColor(230, 230, 230),
        80,
        80,
        80
    };
    QPoint dragPosition;
    QLCDNumber *lcd = new QLCDNumber(this);
};

#endif // CLOCKBODY_H
