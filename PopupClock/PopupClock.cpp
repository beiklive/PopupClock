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
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(SetNumClock()));
    timer->start(1000);
	
	//新建QSystemTrayIcon对象
	mSysTrayIcon = new QSystemTrayIcon(this);
	//新建托盘要显示的icon
	QIcon icon = QIcon(":/PopupClock/DDJ.ico");
	//将icon设到QSystemTrayIcon对象中
	mSysTrayIcon->setIcon(icon);
	//当鼠标移动到托盘上的图标时，会显示此处设置的内容
	mSysTrayIcon->setToolTip(QString::fromUtf8("PopUpClock"));
	//在系统托盘显示此对象
	mSysTrayIcon->show();

	//connect(mSysTrayIcon, &QSystemTrayIcon::activated, this, &PopupClock::activeTray);//点击托盘，执行相应的动作

	auto m_menu = new QMenu(this);
//	auto m_action1 = new QAction(m_menu);
	auto m_action2 = new QAction(m_menu);

//	m_action1->setText("Show Window");
	m_action2->setText("Exit");

//	m_menu->addAction(m_action1);
	m_menu->addAction(m_action2);
	// 
 	connect(m_action2, &QAction::triggered, this, &QApplication::quit);
	// 	connect(m_action2, &QAction::triggered, this, &PopupClock::showMessage);

	mSysTrayIcon->setContextMenu(m_menu);
}



void PopupClock::SetNumClock()
{
    QTime time = QTime::currentTime();
    QString txtTime = time.toString("hh:mm:ss");
    ui.lcdNumber->display(txtTime);

	QString min = time.toString("mm");
	QString sec = time.toString("ss");
	
	if ( min == "29") {
		if (sec == "30")
		{
			MoveClock();
		}
	}

	if (min == "30") {
		if (sec == "30")
		{
			MoveClockback();
		}
	}
}





void PopupClock::MoveClock() {
	auto width = this->width();
	QPropertyAnimation* m_pAnimation;
	m_pAnimation = new QPropertyAnimation(this, "pos");
	m_pAnimation->setDuration(2000);//设置移动时间
	m_pAnimation->setStartValue(QPoint(m_x, 50));//开始位置
	m_pAnimation->setEndValue(QPoint(m_x + width, 50));//结束位置 
	m_pAnimation->start();//开始移动

}

void PopupClock::MoveClockback() {
	auto width = this->width();
	QPropertyAnimation* m_pAnimation;
	m_pAnimation = new QPropertyAnimation(this, "pos");
	m_pAnimation->setDuration(2000);//设置移动时间
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
