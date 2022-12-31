#ifndef SETS_H
#define SETS_H

#include <QWidget>
#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QSettings>
#include <QDir>
#include<QCloseEvent>
#include <QCoreApplication>
#include <QProcess>
#include "PopupClock.h"
namespace Ui {
class Sets;
}

class Sets : public QWidget
{
    Q_OBJECT

public:
    explicit Sets(QWidget *parent = nullptr);
    void GetParent(PopupClock *p);
    ~Sets();

protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_BtnAutoStart_stateChanged(int arg1);


    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_checkBox_clicked(bool checked);

private:
    Ui::Sets *ui;
    const QString AUTO_RUN = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    PopupClock *pare = nullptr;
    int moveSpeed = 1500;
    int keepTime = 60;
    QString Second = "30";
    QString Minute = "29 59";
    QString Hour = "";
    QString Week = "";
    int ClockX = 0;
    int ClockY = 0;
    bool animteState = true;
    QString filePath = "setting.ini";
    QList<QString> *strlist;

private:
    bool checkAutoStart();
    void SetAutoStart(bool flag);

    QList<QString> *splitString(QString str, int MaxNum);

};

#endif // SETS_H
