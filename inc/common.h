#ifndef COMMON_H
#define COMMON_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QStandardPaths>
#include <QString>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QLCDNumber>
#include <QPainterPath>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


const QString kCachePath{QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/PopupClock/"};

class Logger
{
private:
    Logger()
    {
        QString filePath = kCachePath + "logs/rotating.log";
        const std::string kLogFilePath{std::string(filePath.toLocal8Bit())};
        const std::string kLoggerName{"operation_logger"};
        // 创建一个 rotating_file_sink，将日志记录到文件中，每当日志数量达到 10MB 时就切分日志文件
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(kLogFilePath, 10485760, 3);
        // 创建一个日志记录器，并将 rotating_file_sink 传递给它
        m_logger = std::make_shared<spdlog::logger>(kLoggerName, file_sink);
        // 设置日志级别为 debug
        m_logger->set_level(spdlog::level::debug);
        // 设置日志刷新间隔为 1 秒
        m_logger->flush_on(spdlog::level::trace);
        // 将日志同时打印到终端
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        m_logger->sinks().push_back(console_sink);
        m_logger->info("start record");
    };

public:
    static Logger &getInstance()
    {
        static Logger instance;
        return instance;
    }

    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;

    std::shared_ptr<spdlog::logger> getLogger() const
    {
        return m_logger;
    }

        // QStringToString
    std::string QStringToString(const QString &qstr)
    {
        QByteArray cdata = qstr.toLocal8Bit();
        return std::string(cdata);
    }

private:
    std::shared_ptr<spdlog::logger> m_logger;
};

#endif // COMMON_H
