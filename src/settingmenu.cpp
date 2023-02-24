#include "settingmenu.h"
#include "ui_settingmenu.h"

SettingMenu::SettingMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingMenu)
{
    ui->setupUi(this);
    // set window title
    this->setWindowTitle(WINDOW_TITLE);
    // set window icon
    this->setWindowIcon(QIcon(WINDOW_ICON));
}

SettingMenu::~SettingMenu()
{
    delete ui;
}

void SettingMenu::closeEvent(QCloseEvent *event)
{
    logger->info("SettingMenu::closeEvent");
    this->hide();
    event->ignore();
}


void SettingMenu::SetUiToSettingStruct()
{
    settingInfo.isAutoStartSet = ui->autoStartSet->isChecked();
    settingInfo.isAnimationSet = ui->isAinmationSet->isChecked();
    settingInfo.clockMoveDistance = ui->distanceWidget->value();
    settingInfo.clockMoveInterval = ui->intervalWidget->value();
    settingInfo.clockMoveSpeed = ui->speedWidget->value();
    settingInfo.secondList = ui->secondWidget->text();
    settingInfo.minuteList = ui->minuteWidget->text();
    settingInfo.hourList = ui->hourWidget->text();
    settingInfo.dayList = ui->dayWidget->text();
}

void SettingMenu::SetSettingStructToUi(const SettingStruct& sets)
{
    ui->autoStartSet->setChecked(sets.isAutoStartSet);
    ui->isAinmationSet->setChecked(sets.isAnimationSet);
    ui->distanceWidget->setValue(sets.clockMoveDistance);
    ui->intervalWidget->setValue(sets.clockMoveInterval);
    ui->speedWidget->setValue(sets.clockMoveSpeed);
    ui->secondWidget->setText(sets.secondList);
    ui->minuteWidget->setText(sets.minuteList);
    ui->hourWidget->setText(sets.hourList);
    ui->dayWidget->setText(sets.dayList);

    ui->groupBox_2->setEnabled(sets.isAnimationSet);
}

void SettingMenu::on_buttonBox_accepted()
{
    logger->info("SettingMenu::on_buttonBox_accepted");
    SetUiToSettingStruct();

    emit settingInfoSignal(settingInfo);
    this->hide();
}


void SettingMenu::on_buttonBox_rejected()
{
    logger->info("SettingMenu::on_buttonBox_rejected");
    SetSettingStructToUi(settingInfo);
    this->hide();
}

void SettingMenu::setSettingStructSlot(SettingStruct sets)
{
    logger->info("SettingMenu::setSettingStruct");
    SetSettingStructToUi(sets);
    settingInfo = sets;
}