#include "sets.h"
#include "ui_sets.h"
#include <QDebug>
Sets::Sets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sets)
{
    ui->setupUi(this);

    QFile file(filePath);

    if(file.exists())
    {
        qDebug()<<"文件存在";
        auto config = new QSettings(filePath,QSettings::IniFormat);
        animteState = config->value("Config/Animation").toInt() == 0? false : true;
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
        qDebug()<<"文件不存在";
        auto config = new QSettings(filePath,QSettings::IniFormat);
        config->setValue("Config/Animation","0");
        config->setValue("Config/moveSpeed",moveSpeed);
        config->setValue("Config/keeptime",keepTime);
        config->setValue("Config/Second",Second);
        config->setValue("Config/Minute",Minute);
        config->setValue("Config/Hour",Hour);
        config->setValue("Config/Week",Week);
        config->setValue("Config/ClockX",ClockX);
        config->setValue("Config/ClockY",ClockY);

    }
qDebug() << QCoreApplication::applicationDirPath();
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

bool Sets::checkAutoStart() {
    QSettings settings(AUTO_RUN, QSettings::NativeFormat);
    QFileInfo fInfo(QApplication::applicationFilePath());
    QString name = fInfo.baseName();
    QString oldPath = settings.value(name).toString();
    QString newPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    return (settings.contains(name) && newPath == oldPath);
}

void Sets::SetAutoStart(bool flag) {
    QSettings settings(AUTO_RUN, QSettings::NativeFormat);

    QFileInfo fInfo(QApplication::applicationFilePath());
    QString name = fInfo.baseName();

    QString oldPath = settings.value(name).toString();
    QString newPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    if (flag)
    {
        if (oldPath != newPath)
            settings.setValue(name, newPath);
    }
    else {
        settings.remove(name);
    }
//    ui->BtnAutoStart->setChecked(checkAutoStart());
}

void Sets::on_BtnAutoStart_stateChanged(int arg1)
{
    if(arg1 == 2){
        SetAutoStart(true);
    }
    else {
        SetAutoStart(false);
    }
}


// 字符串按空格分割，返回列表
QList<QString>* Sets::splitString(QString str, int MaxNum)
{
    strlist = new QList<QString>;
    if(str.isEmpty())
    {
        for (size_t i = 0; i < MaxNum; i++)
        {
            strlist->append(QString::number(i));
        }
    }else{
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
    if(p != nullptr){
        pare = p;
        pare->SetClockStatus(ClockX, ClockY,animteState,ui->mspeed->value(), ui->ktime->value(), splitString(Second, 60),splitString(Minute, 60),splitString(Hour, 24),splitString(Week, 7));
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

    pare->SetClockStatus(animteState, speed, ktime, splitString(Second, 60),splitString(Minute, 60),splitString(Hour, 24),splitString(Week, 7));
    this->hide();
    auto config = new QSettings(filePath,QSettings::IniFormat);

    config->setValue("Config/Animation",(animteState? "1" : "0"));
    config->setValue("Config/moveSpeed",speed);
    config->setValue("Config/keeptime",ktime);
    config->setValue("Config/Second",Second);
    config->setValue("Config/Minute",Minute);
    config->setValue("Config/Hour",Hour);
    config->setValue("Config/Week",Week);
}


void Sets::on_buttonBox_rejected()
{
    this->hide();
}


void Sets::on_checkBox_clicked(bool checked)
{
    animteState = checked;
}

