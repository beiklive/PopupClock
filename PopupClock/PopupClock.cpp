#pragma execution_character_set("utf-8")
#include "PopupClock.h"
#include <QSystemTrayIcon>
#include <QIcon>
PopupClock::PopupClock(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	m_x = 0 - this->width();
	this->move(QPoint(m_x, 50));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

	QTime time = QTime::currentTime();
	QString txtTime = time.toString("hh:mm:ss");
	ui.lcdNumber->display(txtTime);
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(SetNumClock()));
    timer->start(1000);
	
	//新建QSystemTrayIcon对象
	mSysTrayIcon = new QSystemTrayIcon(this);
	//新建托盘要显示的icon
	QIcon icon = QIcon(":/PopupClock/DDJ.png");
	//将icon设到QSystemTrayIcon对象中
	mSysTrayIcon->setIcon(icon);
	//当鼠标移动到托盘上的图标时，会显示此处设置的内容
	mSysTrayIcon->setToolTip("超级小桀的时钟");
	//在系统托盘显示此对象
	mSysTrayIcon->show();


	auto m_menu = new QMenu(this);
	auto m_action2 = new QAction(m_menu);
	m_action2->setText("Exit");
	m_menu->addAction(m_action2); 
 	connect(m_action2, &QAction::triggered, this, &QApplication::quit);
	mSysTrayIcon->setContextMenu(m_menu);
}



void PopupClock::SetNumClock()
{
    QTime time = QTime::currentTime();
    QString txtTime = time.toString("hh:mm:ss");
    ui.lcdNumber->display(txtTime);

	int min = time.toString("mm").toInt();
	int sec = time.toString("ss").toInt();
	
	if ( min == chargeMin) {
		if (sec == chargeSec)
		{
			MoveClock();
		}
	}

	if (min == chargeMin+1) {
		if (sec == chargeSec)
		{
			MoveClockback();
		}
	}
}





void PopupClock::MoveClock() {
	auto width = this->width();
	QPropertyAnimation* m_pAnimation;
	m_pAnimation = new QPropertyAnimation(this, "pos");
	m_pAnimation->setDuration(1500);//设置移动时间
	m_pAnimation->setStartValue(QPoint(m_x, 50));//开始位置
	m_pAnimation->setEndValue(QPoint(m_x + width, 50));//结束位置 
	m_pAnimation->start();//开始移动

}

void PopupClock::MoveClockback() {
	auto width = this->width();
	QPropertyAnimation* m_pAnimation;
	m_pAnimation = new QPropertyAnimation(this, "pos");
	m_pAnimation->setDuration(1500);//设置移动时间
	m_pAnimation->setStartValue(QPoint(m_x + width, 50));//开始位置
	m_pAnimation->setEndValue(QPoint(m_x, 50));//结束位置 
	m_pAnimation->start();//开始移动
}
void PopupClock::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		last_mouse_position_ = event->globalPos();
}

void PopupClock::mouseMoveEvent(QMouseEvent* event)
{
	if (!event->buttons().testFlag(Qt::LeftButton))
		return;
	const QPoint position = pos() + event->globalPos() - last_mouse_position_;
	move(position.x(), position.y());
	last_mouse_position_ = event->globalPos();
}
