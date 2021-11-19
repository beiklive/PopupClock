#include "DrawClock.h"

DrawClock::DrawClock(QWidget *parent)
	: QWidget(parent)
{
	//����һ����ʱ��
	QTimer* timer = new QTimer(this);
	//������ʱ��������Ϊ1��
	timer->start(500);
	//��ʱ�����ʱ��ִ����Ӧ������������ÿһ����и��´��壬���д����ػ�
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

DrawClock::~DrawClock()
{
}

const QPoint DrawClock::hourHand[4] = {               //�ı���ʱ��ͼ�ε��ĸ��ǵ������
	QPoint(4, 0),//���ĵ㡢�Ҳ�
	QPoint(-4, 0),//��ͷ
	QPoint(-4, -50),//���ĵ㡢���
	QPoint(4, -50)//��β    (ע��ӳ�������ԭ����(110,110))
};
const QPoint DrawClock::minuteHand[4] = {
	QPoint(3, 0),
	QPoint(-3, 0),
	QPoint(-3, -80),
	QPoint(3, -80)
};
const QPoint DrawClock::secondHand[4] = {
	QPoint(1, 0),//�Ҳ�
	QPoint(-1, 0),//���
	QPoint(-1, -100),//��β
	QPoint(1, -100)//��β
};


void DrawClock::paintEvent(QPaintEvent*)
{
	//����ǰ������Ϊ����
	QPainter painter(this);
	//������ݣ�����������Բ��
	painter.setRenderHint(QPainter::Antialiasing, true);
	// ���滭ԭ������ΪΪ��������
	painter.translate(width() / 2, height() / 2);
	//��ȡ ����Ŀ�����ĸ� ����ֵ�Ľ�Сֵ
	int size = width() < height() ? width() : height();
	//���ձ�����������
	painter.scale(size / 220.0, size / 220.0);
	//������
	drawBackgroud(&painter);
	//��ʱ��
	drawHourHand(&painter);
	//������
	drawMinuteHand(&painter);
	//������
	drawsecondHand(&painter);
	//���û�ˢ
	painter.setBrush(Qt::black);
	//�����ĵ�
	drawCentre(&painter);
}
void DrawClock::drawHourHand(QPainter* painter)
{
	//��ȡ��ǰʱ��
	QTime time = QTime::currentTime();
	//���û�ˢ
	painter->setBrush(Qt::black);
	//���û���
	painter->setPen(Qt::black);
	//���浱ǰpainter״̬��ջ�С���Ϊ������ת��Ҫ�ָ�
	painter->save();
	//��ת�Ƕ�  1��Сʱ30��(5���̶�)     ��ǰ���ӵ�Сʱ��   ��ǰ���Сʱ��
	painter->rotate(30.0 * (time.hour() + time.minute() / 60.0) + time.second() / 3600.0);
	//����ʱ��  (���ƶ���ζ����)
	painter->drawConvexPolygon(hourHand, 4);
	//����ͼ�κ�ָ�֮ǰpainter״̬(��ջ)
	painter->restore();
}
void DrawClock::drawMinuteHand(QPainter* painter)
{
	//��ȡ��ǰʱ��
	QTime time = QTime::currentTime();
	//���û�ˢ
	painter->setBrush(Qt::black);
	//���û���
	painter->setPen(Qt::black);
	//���浱ǰpainter״̬��ջ�С���Ϊ������ת��Ҫ�ָ�
	painter->save();
	//��ת�Ƕ�     1����1�̶�(6��)         ��ǰ������Сʱ��
	painter->rotate(6.0 * (time.minute() + time.second() / 60.0));
	//���Ʒ���
	painter->drawConvexPolygon(minuteHand, 4);
	//����ͼ�κ�ָ�֮ǰ״̬(��ջ)
	painter->restore();
}
void DrawClock::drawsecondHand(QPainter* painter)
{
	//��ȡ��ǰʱ��
	QTime time = QTime::currentTime();
	//���û�ˢ
	painter->setBrush(Qt::red);
	//���û���
	painter->setPen(Qt::red);
	//���浱ǰpainter״̬��ջ�С���Ϊ������ת��Ҫ�ָ�
	painter->save();
	//��ת
	painter->rotate(6.0 * time.second());
	//��������
	painter->drawConvexPolygon(secondHand, 4);
	//����ͼ�κ�ָ�֮ǰpainter״̬(��ջ)
	painter->restore();
}

void DrawClock::drawBackgroud(QPainter* painter)
{
	//������ݣ�����������Բ��
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);;
	//���佥��(��Բ�ı߽��䵽����)                    Բ��         �뾶        ����
	QRadialGradient radialGradient(QPointF(0, 0), 110, QPointF(0, 0));
	//��Բ�ı߽��䵽���㣬�̶ȴ�1~0���������ӣ���1~0.9�̶ȴ���ɫ1���䵽��ɫ2����0.9~0.89�̶ȴ���ɫ2���䵽��ɫ3
	radialGradient.setColorAt(1, QColor(230, 230, 230, 255));//��ɫ1
	radialGradient.setColorAt(0.9, QColor(230, 230, 230, 255));//��ɫ2
	radialGradient.setColorAt(0.89, QColor(230, 230, 230, 255));//��ɫ3
	//�߿�����ɫ
	painter->setPen(Qt::NoPen);
	//ʹ�÷��佥����Ϊ��ˢ
	painter->setBrush(radialGradient);
	//��һ��Բ                ��Բ����       ���� ����
	painter->drawEllipse(QPointF(0, 0), 110, 110);
}

void DrawClock::drawCentre(QPainter* painter)
{
	//�����ĵ�
	//�ǶȽ���
	QConicalGradient coneGradient(0, 0, -90.0);
	//������ɫ������λ������
	coneGradient.setColorAt(0.0, Qt::darkGray);
	coneGradient.setColorAt(0.2, QColor(150, 150, 200));
	coneGradient.setColorAt(0.5, Qt::white);
	coneGradient.setColorAt(1.0, Qt::darkGray);
	//û���ߣ�����û�б߽�
	painter->setPen(Qt::NoPen);
	//���û�ˢ
	painter->setBrush(coneGradient);
	//������Բ
	painter->drawEllipse(-5, -5, 10, 10);
}