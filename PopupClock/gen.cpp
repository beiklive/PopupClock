#include "gen.h"
#include "ui_gen.h"
#pragma execution_character_set("utf-8")

gen::gen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gen)
{
//    ui->setupUi(this);
    this->hide();

    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon(":/PopupClock/DDJ.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(("桀哥的时钟"));
    //在系统托盘显示此对象
    mSysTrayIcon->show();

    auto m_menu = new QMenu(this);

    m_pActionAutoStart = new QAction(m_menu);
    m_pActionAutoStart->setText(("设置"));
    connect(m_pActionAutoStart, &QAction::triggered, this, &gen::ShowSetting);
    m_menu->addAction(m_pActionAutoStart);

    m_pActionExit = new QAction(m_menu);
    m_pActionExit->setText(("离开"));
    m_menu->addAction(m_pActionExit);
    connect(m_pActionExit, &QAction::triggered, this, &QApplication::quit);
    mSysTrayIcon->setContextMenu(m_menu);
    clock = new PopupClock(this);
    clock->show();
    settingUI.GetParent(clock);
}

gen::~gen()
{
    delete ui;
}

void gen::ShowSetting()
{
    settingUI.show();
}
