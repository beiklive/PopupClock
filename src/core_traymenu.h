#ifndef GEN_H
#define GEN_H

#include <QObject>
#include <QTimer>
#include <QMouseEvent>
#include <QTime>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
#include <QList>
#include "core_operation.h"
#include "clock_settings.h"
#include "core_logger.hpp"
extern std::shared_ptr<spdlog::logger> logger;
class gen : public QObject
{
    Q_OBJECT

public:
    explicit gen();
    ~gen() = default;

private:
    PopupClock *clock;
    Sets settingUI;

    QSystemTrayIcon *mSysTrayIcon;
    QAction *m_pActionAutoStart;
    QAction *m_pActionExit;

public slots:
    void ShowSetting();
};

#endif // GEN_H
