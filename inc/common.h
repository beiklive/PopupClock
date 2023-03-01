#ifndef _CLOCK_COMMON_H
#define _CLOCK_COMMON_H

#include <QWidget>
#include <QtGlobal>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QStandardPaths>
#include <QStringList>
#include <QString>
#include <QPixmap>
#include <QFile>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QDir>
#include <QSettings>
#include <QFileInfo>
#include <QTimer>
#include <QProcess>
#include <QMouseEvent>
#include <QLCDNumber>
#include <QPainterPath>
#include <QPropertyAnimation>

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#ifdef Q_OS_WIN
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

const QString kCachePath{QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/PopupClock/"};
const QString kConfigPath{QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/PopupClock/config.ini"};

struct ClockBodyStruct
{
    int width;
    int height;
    QColor backgroundColor;
    int borderRadius;
};
struct ClockDialStruct
{
    int width;
    int height;
    QColor backgroundColor;
    QColor secondHandColor;
    QColor minuteHandColor;
    QColor hourHandColor;
    int positionX;
    int positionY;
};
struct ClockNumberStruct
{
    int width;
    int height;
    QColor backgroundColor;
    int borderRadius;
    int positionX;
    int positionY;
};
struct ConfigStruct
{
    int positionX;
    int positionY;
    int size;
    bool isAutoStartSet;
    bool isAnimationSet;
    int clockMoveSpeed;
    int clockMoveInterval;
    int clockMoveDistance;
    QString secondList;
    QString minuteList;
    QString hourList;
    QString dayList;
};

enum ClockBodyState
{
    CLOCKBODY_MOVE_TO_SHOW,
    CLOCKBODY_SHOW,
    CLOCKBODY_MOVE_TO_HIDE,
    CLOCKBODY_HIDE
};

struct ClockBodyStateStruct
{
    ClockBodyState currentState;
    ClockBodyState targetState;
};

enum StateConditionGuard
{
    GUARD_OPEN,
    GUARD_CLOSE
};
struct SettingStruct
{
    bool isAutoStartSet;
    bool isAnimationSet;
    int size;
    int clockMoveSpeed;
    int clockMoveInterval;
    int clockMoveDistance;
    QString secondList;
    QString minuteList;
    QString hourList;
    QString dayList;
};

/* 配置文件管理类，单例模式，使用方法如下：
    ConfigManager& config = ConfigManager::Instance();
    config.LoadConfig("config.ini");
    config.SetValue("key", "value");
    auto value = config.GetValue("key");
    config.SaveConfig();
*/
class ConfigManager
{
public:
    static ConfigManager &Instance()
    {
        static ConfigManager instance; // 单例实例
        return instance;
    }

    // 读取配置文件
    void LoadConfig(const std::string &filename)
    {
        m_configPath = filename;
        std::ifstream file(filename.c_str());
        if (!file.good())
        {
            // 如果文件不存在，则创建空文件
            std::ofstream ofs(filename.c_str());
            if (!ofs.good())
            {
                throw std::runtime_error("Failed to create config file!");
            }
            m_isConfigFirstLoad = true;
            ofs.close();
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            size_t pos = line.find('=');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                m_configs[key] = value;
            }
        }
    }

    // 获取配置项的值
    std::string GetValue(const std::string &key, const std::string &defaultValue = "") const
    {
        auto it = m_configs.find(key);
        if (it != m_configs.end())
        {
            return it->second;
        }
        return defaultValue;
    }

    // 设置配置项的值
    void SetValue(const std::string &key, const std::string &value)
    {
        m_configs[key] = value;
    }

    // 保存配置到文件
    void SaveConfig() const
    {
        std::ofstream file(m_configPath.c_str());
        for (const auto &[key, value] : m_configs)
        {
            file << key << "=" << value << std::endl;
        }

        SyncFile(file);
    }

    // 判断配置文件是否是第一次加载
    const bool IsConfigFirstLoad() const
    {
        return m_isConfigFirstLoad;
    }

private:
    std::map<std::string, std::string> m_configs; // 存储配置项的键值对
    std::string m_configPath;
    bool m_isConfigFirstLoad = false;
    ConfigManager() = default;                                // 禁止外部创建实例
    ConfigManager(const ConfigManager &) = delete;            // 禁止复制构造函数
    ConfigManager &operator=(const ConfigManager &) = delete; // 禁止复制赋值运算符

    // 同步文件到磁盘
    void SyncFile(std::ofstream &file) const
    {
        file.flush(); // 刷新缓存
#ifdef Q_OS_WIN
        HANDLE hFile = (HANDLE)_get_osfhandle(file.rdstate()); // 获取文件句柄
        FlushFileBuffers(hFile);                               // 强制刷新文件到磁盘
#else
        int fd = file.rdstate(); // 获取文件描述符
        fsync(fd);               // 强制刷新文件到磁盘
#endif
    }
};

// 日志类，单例模式，使用方法如下：
// Logger::getInstance().getLogger()->info("Hello, {}!", "world");
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
        m_logger->set_level(spdlog::level::info);
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
