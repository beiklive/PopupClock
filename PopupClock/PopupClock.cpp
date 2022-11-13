#include "PopupClock.h"
#include <QSystemTrayIcon>
#include <QIcon>
#pragma execution_character_set("utf-8")
#define AUTO_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

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
	mSysTrayIcon->setToolTip(QString::fromLocal8Bit("桀哥的时钟"));
	//在系统托盘显示此对象
	mSysTrayIcon->show();

	auto m_menu = new QMenu(this);
	
	m_pActionAutoStart = new QAction(m_menu);
	m_pActionAutoStart->setText(QString::fromLocal8Bit("开机自启"));
	m_pActionAutoStart->setCheckable(true);
	m_pActionAutoStart->setChecked(checkAutoStart());
	connect(m_pActionAutoStart, &QAction::triggered, this, &PopupClock::SetAutoStart);
	m_menu->addAction(m_pActionAutoStart);
	
	m_pActionExit = new QAction(m_menu);
	m_pActionExit->setText(QString::fromLocal8Bit("离开"));
	m_menu->addAction(m_pActionExit);
	connect(m_pActionExit, &QAction::triggered, this, &QApplication::quit);
	mSysTrayIcon->setContextMenu(m_menu);
}



void PopupClock::SetNumClock()
{
    QTime time = QTime::currentTime();
    QString txtTime = time.toString("hh:mm:ss");
    ui.lcdNumber->display(txtTime);

	int min = time.toString("mm").toInt();
	int sec = time.toString("ss").toInt();
	
	if ( min == chargeMin1 || min == chargeMin2) {
		if (sec == chargeSec)
		{
			MoveClock();
		}
	}

	if (min == chargeMin1 + 1 || min == (chargeMin2 + 1)%60) {
		if (sec == chargeSec)
		{
			MoveClockback();
		}
	}
}

void PopupClock::SetAutoStart(bool flag) {
	QSettings settings(AUTO_RUN, QSettings::NativeFormat);

	//以程序名称作为注册表中的键,根据键获取对应的值（程序路径）
	QFileInfo fInfo(QApplication::applicationFilePath());
	QString name = fInfo.baseName();    //键-名称

	//如果注册表中的路径和当前程序路径不一样，则表示没有设置自启动或本自启动程序已经更换了路径
	QString oldPath = settings.value(name).toString(); //获取目前的值-绝对路经
	QString newPath = QDir::toNativeSeparators(QApplication::applicationFilePath());    //toNativeSeparators函数将"/"替换为"\"
	if (flag)
	{
		if (oldPath != newPath)
			settings.setValue(name, newPath);
	}
	else {
		settings.remove(name);
	}
	m_pActionAutoStart->setChecked(checkAutoStart());
}

bool PopupClock::checkAutoStart() {
	QSettings settings(AUTO_RUN, QSettings::NativeFormat);
	QFileInfo fInfo(QApplication::applicationFilePath());
	QString name = fInfo.baseName();
	QString oldPath = settings.value(name).toString();
	QString newPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
	return (settings.contains(name) && newPath == oldPath);
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
