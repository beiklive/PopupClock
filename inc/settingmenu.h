#ifndef SETTINGMENU_H
#define SETTINGMENU_H
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "common.h"
extern std::shared_ptr<spdlog::logger> logger;
extern ConfigManager& Config;
class SettingMenu : public QWidget
{
    Q_OBJECT
public:
    SettingMenu(QWidget *parent = nullptr);
    ~SettingMenu();

public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *BtnAutoStart;
    QCheckBox *checkBox;
    QFrame *line;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QLabel *label_2;
    QVBoxLayout *verticalLayout_2;
    QSpinBox *mspeed;
    QSpinBox *ktime;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QLineEdit *Second;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *Minute;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *Hour;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *Week;
    QHBoxLayout *horizontalLayout_6;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *Sets)
    {
        if (Sets->objectName().isEmpty())
            Sets->setObjectName("Sets");
        Sets->resize(257, 395);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PopupClock/DDJ.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Sets->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(Sets);
        verticalLayout->setObjectName("verticalLayout");
        groupBox_4 = new QGroupBox(Sets);
        groupBox_4->setObjectName("groupBox_4");
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setObjectName("verticalLayout_4");
        BtnAutoStart = new QCheckBox(groupBox_4);
        BtnAutoStart->setObjectName("BtnAutoStart");

        verticalLayout_4->addWidget(BtnAutoStart);

        checkBox = new QCheckBox(groupBox_4);
        checkBox->setObjectName("checkBox");

        verticalLayout_4->addWidget(checkBox);

        verticalLayout->addWidget(groupBox_4);

        line = new QFrame(Sets);
        line->setObjectName("line");
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        groupBox = new QGroupBox(Sets);
        groupBox->setObjectName("groupBox");
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        verticalLayout_3->addWidget(label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        verticalLayout_3->addWidget(label_2);

        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        mspeed = new QSpinBox(groupBox);
        mspeed->setObjectName("mspeed");
        mspeed->setMaximum(9999);

        verticalLayout_2->addWidget(mspeed);

        ktime = new QSpinBox(groupBox);
        ktime->setObjectName("ktime");
        ktime->setMaximum(120);

        verticalLayout_2->addWidget(ktime);

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_5->addLayout(horizontalLayout);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_6 = new QVBoxLayout(groupBox_2);
        verticalLayout_6->setObjectName("verticalLayout_6");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName("label_6");

        horizontalLayout_2->addWidget(label_6);

        Second = new QLineEdit(groupBox_2);
        Second->setObjectName("Second");

        horizontalLayout_2->addWidget(Second);

        verticalLayout_6->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");

        horizontalLayout_3->addWidget(label_3);

        Minute = new QLineEdit(groupBox_2);
        Minute->setObjectName("Minute");

        horizontalLayout_3->addWidget(Minute);

        verticalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");

        horizontalLayout_4->addWidget(label_4);

        Hour = new QLineEdit(groupBox_2);
        Hour->setObjectName("Hour");

        horizontalLayout_4->addWidget(Hour);

        verticalLayout_6->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName("label_5");

        horizontalLayout_5->addWidget(label_5);

        Week = new QLineEdit(groupBox_2);
        Week->setObjectName("Week");

        horizontalLayout_5->addWidget(Week);

        verticalLayout_6->addLayout(horizontalLayout_5);

        verticalLayout_5->addWidget(groupBox_2);

        verticalLayout->addWidget(groupBox);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        buttonBox = new QDialogButtonBox(Sets);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

        horizontalLayout_6->addWidget(buttonBox);

        verticalLayout->addLayout(horizontalLayout_6);

        retranslateUi(Sets);
        QObject::connect(checkBox, &QCheckBox::clicked, groupBox, &QGroupBox::setEnabled);

        QMetaObject::connectSlotsByName(Sets);
    } // setupUi

    void retranslateUi(QWidget *Sets)
    {
        Sets->setWindowTitle(QCoreApplication::translate("Sets", "\350\256\276\347\275\256", nullptr));
        groupBox_4->setTitle(QString());
        BtnAutoStart->setText(QCoreApplication::translate("Sets", "\345\274\200\346\234\272\350\207\252\345\220\257\345\212\250", nullptr));
        checkBox->setText(QCoreApplication::translate("Sets", "\346\212\275\345\261\211\345\212\250\347\224\273\346\225\210\346\236\234", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Sets", "\345\212\250\347\224\273\346\225\210\346\236\234\350\256\276\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("Sets", "\347\247\273\345\212\250\346\227\266\351\227\264(\346\257\253\347\247\222)\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Sets", "\345\201\234\347\225\231\346\227\266\351\227\264(\347\247\222)\357\274\232", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Sets", "\346\227\266\351\227\264\350\256\276\347\275\256\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("Sets", "\347\247\222\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("Sets", "\345\210\206\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("Sets", "\346\227\266\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("Sets", "\345\221\250\357\274\232", nullptr));
    } // retranslateUi
};

#endif // SETTINGMENU_H
