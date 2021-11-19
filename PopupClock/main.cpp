#include "PopupClock.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // a.setQuitOnLastWindowClosed(false);
    PopupClock w;
    w.show();
    return a.exec();
}
