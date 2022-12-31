#include "gen.h"
#pragma execution_character_set("utf-8")

gen::gen()
{

    mSysTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/PopupClock/DDJ.png");
    mSysTrayIcon->setIcon(icon);
    mSysTrayIcon->setToolTip(("桀哥的时钟 仿"));
    mSysTrayIcon->show();

    auto m_menu = new QMenu();

    m_pActionAutoStart = new QAction(m_menu);
    m_pActionAutoStart->setText(("设置"));
    connect(m_pActionAutoStart, &QAction::triggered, this, &gen::ShowSetting);
    m_menu->addAction(m_pActionAutoStart);

    m_pActionExit = new QAction(m_menu);
    m_pActionExit->setText(("离开"));
    m_menu->addAction(m_pActionExit);
    connect(m_pActionExit, &QAction::triggered, this, &QApplication::quit);
    mSysTrayIcon->setContextMenu(m_menu);
    clock = new PopupClock();
    clock->show();
    settingUI.GetParent(clock);

}

gen::~gen()
{
}

void gen::ShowSetting()
{
    settingUI.show();
    clock->ShowClock();
}
