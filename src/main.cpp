
#include <QtWidgets/QApplication>
#include "core_traymenu.h"
#include "core_logger.hpp"

std::shared_ptr<spdlog::logger> logger = Logger::getInstance().getLogger();

int main(int argc, char *argv[])
{

    // 使用日志记录器记录日志
    logger->info("helloworld");
    QApplication a(argc, argv);
    gen w;

    return a.exec();
}
