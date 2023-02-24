#include <QApplication>
#include "common.h"
#include "traymenu.h"


std::shared_ptr<spdlog::logger> logger = Logger::getInstance().getLogger();
int main(int argc, char *argv[])
{
    logger->info("main");
    QApplication a(argc, argv);
    TrayMenu w;
    return a.exec();
}
