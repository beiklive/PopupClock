#include "clockbody.h"

ClockBody::ClockBody(QWidget *parent)
    : QWidget{parent}
{
    initWidget();
    // 设置定时器，0.5秒update()一次
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]()
            { this->update(); });
    timer->start(500);
}

ClockBody::~ClockBody()
{
}

void ClockBody::initWidget()
{
    logger->info("[ClockBody] initWidget");
    this->setFixedSize(clockBodyInfo.width, clockBodyInfo.height);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    this->setAttribute(Qt::WA_TranslucentBackground);

    lcd->setGeometry(tClockNumberFontPosX, tClockNumberFontPosY, tClockNumberFontWidth, tClockNumberFontHeight);
    lcd->setDigitCount(8);
    lcd->setFrameStyle(QFrame::NoFrame);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    lcd->setStyleSheet("color: black;");
}

void ClockBody::paintEvent(QPaintEvent *event)
{
    QTime time = QTime::currentTime();
     drawClockBody(time);
     drawClockDial(time);
     drawClockNumber(time);
}

void ClockBody::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        logger->info("[ClockBody] mousePressEvent position: {}, {}",
                     event->globalPos().x(), event->globalPos().y());
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();

        emit mousePressSignal();
    }
}

void ClockBody::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void ClockBody::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        logger->info("[ClockBody] mouseReleaseEvent position: {}, {}",
                     event->globalPos().x(), event->globalPos().y());
        dragPosition = QPoint();
        event->accept();

        emit mouseReleaseSignal(this->pos());
    }
}

void ClockBody::drawClockBody(const QTime &time)
{
    // logger->info("[ClockBody] drawClockBody");
    // 创建缓冲区
    QPixmap pixmap(QSize(clockBodyInfo.width, clockBodyInfo.height));
    pixmap.fill(Qt::transparent);

    // 在缓冲区中绘制图形
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(clockBodyInfo.backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), clockBodyInfo.borderRadius, clockBodyInfo.borderRadius);

    // 将缓冲区绘制到窗口中
    painter.end();
    painter.begin(this);
    painter.drawPixmap(0, 0, pixmap);
}

void ClockBody::drawClockDial(const QTime &time)
{
    // logger->info("[ClockBody] drawClockDial");
    // 创建一个大小与窗口相同的圆形缓冲区
    QPixmap buffer(QSize(clockDialInfo.width, clockDialInfo.height));
    buffer.fill(Qt::transparent);

    // 在缓冲区中绘制图形
    QPainter painter(&buffer);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(clockDialInfo.width / 2, clockDialInfo.height / 2);
    int size = qMin(clockDialInfo.width, clockDialInfo.height);
    painter.scale(size / 220.0, size / 220.0);
    drawBackgroud(&painter);
    drawHourHand(&painter, time);
    drawMinuteHand(&painter, time);
    drawsecondHand(&painter, time);
    drawCentre(&painter);

    // 将缓冲区中的图形绘制到屏幕上
    QPainter screenPainter(this);
    screenPainter.setRenderHint(QPainter::Antialiasing, true);
    QRectF targetRect(clockDialInfo.positionX, clockDialInfo.positionY, buffer.width(), buffer.height());
    screenPainter.drawPixmap(targetRect, buffer, buffer.rect());
}

void ClockBody::drawHourHand(QPainter *painter, const QTime &time)
{
    painter->setBrush(clockDialInfo.hourHandColor);
    painter->setPen(clockDialInfo.hourHandColor);
    painter->save();
    painter->rotate(30.0 * time.hour() + (time.minute() * 60 + time.second()) / 120.0);
    painter->drawConvexPolygon(hourHand, 4);
    painter->restore();
}
void ClockBody::drawMinuteHand(QPainter *painter, const QTime &time)
{
    painter->setBrush(clockDialInfo.minuteHandColor);
    painter->setPen(clockDialInfo.minuteHandColor);
    painter->save();
    painter->rotate(6.0 * time.minute() + time.second() / 10.0);
    painter->drawConvexPolygon(minuteHand, 4);
    painter->restore();
}
void ClockBody::drawsecondHand(QPainter *painter, const QTime &time)
{
    painter->setBrush(clockDialInfo.secondHandColor);
    painter->setPen(clockDialInfo.secondHandColor);
    painter->save();
    painter->rotate(6.0 * time.second());
    painter->drawConvexPolygon(secondHand, 4);
    painter->restore();
}
void ClockBody::drawBackgroud(QPainter *painter)
{
    painter->setBrush(Qt::black);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRadialGradient radialGradient(QPointF(0, 0), 110, QPointF(0, 0));
    radialGradient.setColorAt(1, clockDialInfo.backgroundColor);
    radialGradient.setColorAt(0.9, clockDialInfo.backgroundColor);
    radialGradient.setColorAt(0.89, clockDialInfo.backgroundColor);
    painter->setPen(Qt::NoPen);
    painter->setBrush(radialGradient);
    painter->drawEllipse(QPointF(0, 0), 110, 110);
}

void ClockBody::drawCentre(QPainter *painter)
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



void ClockBody::drawClockNumber(const QTime &time)
{
    // 创建缓冲区
    QPixmap buffer(QSize(clockNumberInfo.width, clockNumberInfo.height));
    buffer.fill(Qt::transparent);
    // 创建一个QPainter对象，用于在QPixmap上绘制图形
    QPainter bufferPainter(&buffer);
    // 启用抗锯齿渲染
    bufferPainter.setRenderHint(QPainter::Antialiasing, true);

    // 在QPixmap上绘制长方形
    QRect rect(0, 0, clockNumberInfo.width, clockNumberInfo.height);
    QPainterPath path;
    path.addRoundedRect(rect, clockNumberInfo.borderRadius, clockNumberInfo.borderRadius);
    bufferPainter.setPen(Qt::NoPen);
    bufferPainter.fillPath(path, QBrush(clockNumberInfo.backgroundColor));
    bufferPainter.drawPath(path);

    // 将绘制好的QPixmap绘制到屏幕上
    QPainter screenPainter(this);
    // screenPainter.drawPixmap(rect(), buffer, rect);
    QRectF targetRect(clockNumberInfo.positionX, clockNumberInfo.positionY, buffer.width(), buffer.height());
    screenPainter.drawPixmap(targetRect, buffer, buffer.rect());


    lcd->display(time.toString("hh:mm:ss"));
}
void ClockBody::initLayout(int value){
    mSizeStep = value;
    gap = 3 * value;
    fontGapX = 6.5 * value;
    fontGapY = 4 * value;

    clockDialInfo.height = 20 * value;
    clockDialInfo.width = clockDialInfo.height;

    clockNumberInfo.width = (19 / 6.0) * clockDialInfo.height;
    clockNumberInfo.height = clockDialInfo.height;

    clockBodyInfo.width = 3 * gap + clockDialInfo.width + clockNumberInfo.width;
    clockBodyInfo.height = 2 * gap + clockDialInfo.height;
    clockBodyInfo.borderRadius = 6 * value;

    clockDialInfo.positionX = gap;
    clockDialInfo.positionY = gap;
    clockNumberInfo.positionX = 2 * gap + clockDialInfo.width;
    clockNumberInfo.positionY = gap;
    clockNumberInfo.borderRadius = 6 * value;

    tClockNumberFontPosX = clockNumberInfo.positionX + fontGapX;
    tClockNumberFontPosY = clockNumberInfo.positionY + fontGapY;
    tClockNumberFontWidth = clockNumberInfo.width - fontGapX * 2;
    tClockNumberFontHeight = clockNumberInfo.height - fontGapY * 2;
    this->setFixedSize(clockBodyInfo.width, clockBodyInfo.height);
    lcd->setGeometry(tClockNumberFontPosX, tClockNumberFontPosY, tClockNumberFontWidth, tClockNumberFontHeight);


}
void ClockBody::bodySizeSlot(int value)
{ 
    initLayout(value);
    update();
}
