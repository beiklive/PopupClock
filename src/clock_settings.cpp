#include "clock_settings.h"
#include "ui_sets.h"
#include <QDebug>

Sets::Sets(QWidget *parent) : QWidget(parent),
                              ui(new Ui::Sets)
{
    ui->setupUi(this);
    QFile file(filePath);
    if (file.exists())
    {
        logger->info("[Config Event] config exists");
        auto config = new QSettings(filePath, QSettings::IniFormat);
        animteState = config->value("Config/Animation").toInt() == 0 ? false : true;
        moveSpeed = config->value("Config/moveSpeed").toInt();
        keepTime = config->value("Config/keeptime").toInt();
        Second = config->value("Config/Second").toString();
        Minute = config->value("Config/Minute").toString();
        Hour = config->value("Config/Hour").toString();
        Week = config->value("Config/Week").toString();
        ClockX = config->value("Config/ClockX").toInt();
        ClockY = config->value("Config/ClockY").toInt();
    }
    else
    {
        logger->info("[Config Event] create config");
        auto config = new QSettings(filePath, QSettings::IniFormat);
        config->setValue("Config/Animation", "1");
        config->setValue("Config/moveSpeed", moveSpeed);
        config->setValue("Config/keeptime", keepTime);
        config->setValue("Config/Second", Second);
        config->setValue("Config/Minute", Minute);
        config->setValue("Config/Hour", Hour);
        config->setValue("Config/Week", Week);
        config->setValue("Config/ClockX", ClockX);
        config->setValue("Config/ClockY", ClockY);
        config->sync();
    }
    logger->info("[Config Event] Data Init animteState({}) checkAutoStart({}) moveSpeed({}) keepTime({}) Second({}) Minute({}) Hour({}) Week({})", \
                                                animteState, checkAutoStart(), moveSpeed, keepTime, std::string(Second.toLocal8Bit()), std::string(Minute.toLocal8Bit()), std::string(Hour.toLocal8Bit()), std::string(Week.toLocal8Bit()));
    ui->checkBox->setChecked(animteState);
    ui->BtnAutoStart->setChecked(checkAutoStart());
    ui->groupBox->setEnabled(animteState);
    ui->mspeed->setValue(moveSpeed);
    ui->ktime->setValue(keepTime);
    ui->Second->setText(Second);
    ui->Minute->setText(Minute);
    ui->Hour->setText(Hour);
    ui->Week->setText(Week);
}

Sets::~Sets()
{
    delete ui;
}

void Sets::closeEvent(QCloseEvent *e)
{
    e->ignore();
    logger->info("[Close Event]");
    on_buttonBox_rejected();
}

bool Sets::checkAutoStart()
{
    QSettings settings(AUTO_RUN, QSettings::NativeFormat);
    QFileInfo fInfo(QApplication::applicationFilePath());
    QString name = fInfo.baseName();
    QString oldPath = settings.value(name).toString();
    QString newPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    return (settings.contains(name) && newPath == oldPath);
}

void Sets::SetAutoStart(bool flag)
{
    QSettings settings(AUTO_RUN, QSettings::NativeFormat);

    QFileInfo fInfo(QApplication::applicationFilePath());
    QString name = fInfo.baseName();

    QString oldPath = settings.value(name).toString();
    QString newPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    if (flag)
    {
        if (oldPath != newPath)
        {
            settings.setValue(name, newPath);
            logger->info("[Config Event] Set auto start");
        }
    }
    else
    {
        settings.remove(name);
        logger->info("[Config Event] Cancel auto start");
    }
}

void Sets::on_BtnAutoStart_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        SetAutoStart(true);
    }
    else
    {
        SetAutoStart(false);
    }
}

// 字符串按空格分割，返回列表
QList<QString> *Sets::splitString(QString str, int MaxNum)
{
    strlist = new QList<QString>;
    if (str.isEmpty())
    {
        for (size_t i = 0; i < MaxNum; i++)
        {
            strlist->append(QString::number(i));
        }
    }
    else
    {
        int start = 0;
        int end = 0;
        while (end != -1)
        {
            end = str.indexOf(" ", start);
            if (end == -1)
            {
                strlist->append(str.mid(start));
            }
            else
            {
                strlist->append(str.mid(start, end - start));
                start = end + 1;
            }
        }
    }
    return strlist;
}

void Sets::GetParent(PopupClock *p)
{
    if (p != nullptr)
    {
        pare = p;
        pare->SetClockStatus(ClockX, ClockY, animteState, ui->mspeed->value(), ui->ktime->value(), splitString(Second, 60), splitString(Minute, 60), splitString(Hour, 24), splitString(Week, 7));
        pare->move(ClockX, ClockY);
        if (animteState)
            pare->MoveClockback();
    }
}

void Sets::on_buttonBox_accepted()
{
    int speed = ui->mspeed->value();
    int ktime = ui->ktime->value();

    Second = ui->Second->text();
    Minute = ui->Minute->text();

    Hour = ui->Hour->text();
    Week = ui->Week->text();

    this->hide();
    auto config = new QSettings(filePath, QSettings::IniFormat);
    config->setValue("Config/Animation", (animteState ? "1" : "0"));
    config->setValue("Config/moveSpeed", speed);
    config->setValue("Config/keeptime", ktime);
    config->setValue("Config/Second", Second);
    config->setValue("Config/Minute", Minute);
    config->setValue("Config/Hour", Hour);
    config->setValue("Config/Week", Week);
    config->sync();

    logger->info("[Config Event] Data Set animteState({}) checkAutoStart({}) moveSpeed({}) keepTime({}) Second({}) Minute({}) Hour({}) Week({})", \
                                                animteState, checkAutoStart(), moveSpeed, keepTime, std::string(Second.toLocal8Bit()), std::string(Minute.toLocal8Bit()), std::string(Hour.toLocal8Bit()), std::string(Week.toLocal8Bit()));

    pare->SetClockStatus(pare->pos().x(), pare->pos().y(), animteState, speed, ktime, splitString(Second, 60), splitString(Minute, 60), splitString(Hour, 24), splitString(Week, 7));
    logger->info("[Restart Event]");
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}

void Sets::on_buttonBox_rejected()
{
    this->hide();
    logger->info("[Restart Event]");
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}

void Sets::on_checkBox_clicked(bool checked)
{
    logger->info("[Config Event] Set animteState ({}) -> ({})", animteState, checked);
    animteState = checked;
}
