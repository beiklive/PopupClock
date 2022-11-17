
#include <QtWidgets/QApplication>
#include "gen.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gen w;

    return a.exec();
}
