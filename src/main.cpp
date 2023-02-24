#include <QApplication>
#include "common.h"
#include "traymenu.h"


std::shared_ptr<spdlog::logger> logger = Logger::getInstance().getLogger();
ConfigManager& Config = ConfigManager::Instance();
int main(int argc, char *argv[])
{
    Config.LoadConfig(Logger::getInstance().QStringToString(kConfigPath));
    QApplication a(argc, argv);
    TrayMenu w;
    return a.exec();
}
