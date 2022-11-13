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
	
	//�½�QSystemTrayIcon����
	mSysTrayIcon = new QSystemTrayIcon(this);
	//�½�����Ҫ��ʾ��icon
	QIcon icon = QIcon(":/PopupClock/DDJ.png");
	//��icon�赽QSystemTrayIcon������
	mSysTrayIcon->setIcon(icon);
	//������ƶ��������ϵ�ͼ��ʱ������ʾ�˴����õ�����
	mSysTrayIcon->setToolTip(QString::fromLocal8Bit("����ʱ��"));
	//��ϵͳ������ʾ�˶���
	mSysTrayIcon->show();

	auto m_menu = new QMenu(this);
	
	m_pActionAutoStart = new QAction(m_menu);
	m_pActionAutoStart->setText(QString::fromLocal8Bit("��������"));
	m_pActionAutoStart->setCheckable(true);
	m_pActionAutoStart->setChecked(checkAutoStart());
	connect(m_pActionAutoStart, &QAction::triggered, this, &PopupClock::SetAutoStart);
	m_menu->addAction(m_pActionAutoStart);
	
	m_pActionExit = new QAction(m_menu);
	m_pActionExit->setText(QString::fromLocal8Bit("�뿪"));
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

	//�Գ���������Ϊע����еļ�,���ݼ���ȡ��Ӧ��ֵ������·����
	QFileInfo fInfo(QApplication::applicationFilePath());
	QString name = fInfo.baseName();    //��-����

	//���ע����е�·���͵�ǰ����·����һ�������ʾû�������������������������Ѿ�������·��
	QString oldPath = settings.value(name).toString(); //��ȡĿǰ��ֵ-����·��
	QString newPath = QDir::toNativeSeparators(QApplication::applicationFilePath());    //toNativeSeparators������"/"�滻Ϊ"\"
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
	m_pAnimation->setDuration(1500);//�����ƶ�ʱ��
	m_pAnimation->setStartValue(QPoint(m_x, 50));//��ʼλ��
	m_pAnimation->setEndValue(QPoint(m_x + width, 50));//����λ�� 
	m_pAnimation->start();//��ʼ�ƶ�

}

void PopupClock::MoveClockback() {
	auto width = this->width();
	QPropertyAnimation* m_pAnimation;
	m_pAnimation = new QPropertyAnimation(this, "pos");
	m_pAnimation->setDuration(1500);//�����ƶ�ʱ��
	m_pAnimation->setStartValue(QPoint(m_x + width, 50));//��ʼλ��
	m_pAnimation->setEndValue(QPoint(m_x, 50));//����λ�� 
	m_pAnimation->start();//��ʼ�ƶ�
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
