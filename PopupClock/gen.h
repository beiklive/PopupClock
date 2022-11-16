#ifndef GEN_H
#define GEN_H

#include <QWidget>
#include "PopupClock.h"
#include "sets.h"
namespace Ui {
class gen;
}

class gen : public QWidget
{
    Q_OBJECT

public:
    explicit gen(QWidget *parent = nullptr);
    ~gen();

private:
    Ui::gen *ui;
    PopupClock *clock;
    Sets settingUI;


    QSystemTrayIcon* mSysTrayIcon;
    QAction* m_pActionAutoStart;
    QAction* m_pActionExit;


public slots:
    void ShowSetting();
};

#endif // GEN_H
