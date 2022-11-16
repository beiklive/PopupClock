
#include <QtWidgets/QApplication>
#include "gen.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // a.setQuitOnLastWindowClosed(false);
    gen w;

    return a.exec();
}
