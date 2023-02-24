#ifndef SETTINGMENU_H
#define SETTINGMENU_H

#include "common.h"
extern std::shared_ptr<spdlog::logger> logger;

namespace Ui {
class SettingMenu;
}

class SettingMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SettingMenu(QWidget *parent = nullptr);
    ~SettingMenu();

private:
    void SetUiToSettingStruct();
    void SetSettingStructToUi(const SettingStruct& sets);

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void setSettingStructSlot(SettingStruct sets);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void settingInfoSignal(SettingStruct sets);

private:
    Ui::SettingMenu *ui;

    SettingStruct settingInfo{
        false,
        false,
        0,
        0,
        0,
        "",
        "",
        "",
        ""
    };

    const QString WINDOW_TITLE = "设置";
    const QString WINDOW_ICON = "://setting.jpg";
};

#endif // SETTINGMENU_H
