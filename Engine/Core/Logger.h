// @file Logger.h

#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Engine
{
enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

struct LogRecord
{
    LogLevel level = LogLevel::INFO;
    std::string message;
    const char* file = nullptr;
    int line = 0;
    const char* function = nullptr;
    std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
};

class LogSink
{
   public:
    explicit LogSink(LogLevel minLevel = LogLevel::DEBUG) : minLevel(minLevel) {}
    virtual ~LogSink() = default;
    virtual void log(const LogRecord& record) = 0;

    bool ShouldLog(LogLevel level) const
    {
        return static_cast<int>(level) >= static_cast<int>(minLevel);
    }

   private:
    LogLevel minLevel = LogLevel::DEBUG;
};

class LogFormatter
{
   public:
    static std::string LevelToString(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARNING:
                return "WARN";
            case LogLevel::ERROR:
                return "ERROR";
            default:
                return "UNKNOWN";
        }
    }

    static std::string FormatConsole(const LogRecord& record)
    {
        return "[" + FormatTime(record.time, false) + "] [" + LevelToString(record.level) + "] " +
               record.message;
    }

    static std::string FormatFile(const LogRecord& record)
    {
        std::ostringstream stream;
        stream << "[" << FormatTime(record.time, true) << "] [" << LevelToString(record.level)
               << "] " << ShortFileName(record.file) << ":" << record.line;

        if (record.function != nullptr)
        {
            stream << " (" << record.function << ")";
        }

        stream << " | " << record.message;
        return stream.str();
    }

   private:
    static std::string FormatTime(std::chrono::system_clock::time_point time, bool includeDate)
    {
        const auto timeValue = std::chrono::system_clock::to_time_t(time);
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                                      time.time_since_epoch()) %
                                  1000;

        std::tm localTime = {};
        localtime_s(&localTime, &timeValue);

        std::ostringstream stream;

        if (includeDate)
        {
            stream << std::put_time(&localTime, "%Y-%m-%d ");
        }

        stream << std::put_time(&localTime, "%H:%M:%S") << "." << std::setfill('0')
               << std::setw(3) << milliseconds.count();

        return stream.str();
    }

    static std::string ShortFileName(const char* file)
    {
        if (file == nullptr)
        {
            return "unknown";
        }

        std::filesystem::path path(file);
        return path.filename().string();
    }
};

class ConsoleSink : public LogSink
{
   public:
    explicit ConsoleSink(LogLevel minLevel = LogLevel::INFO) : LogSink(minLevel) {}

    void log(const LogRecord& record) override
    {
        std::cout << LogFormatter::FormatConsole(record) << std::endl;
    }
};

class FileSink : public LogSink
{
   public:
    explicit FileSink(const std::string& filename, LogLevel minLevel = LogLevel::DEBUG)
        : LogSink(minLevel)
    {
        std::filesystem::path logPath(filename);
        std::filesystem::create_directories(logPath.parent_path());

        logFile.open(filename, std::ios::out);
    }

    ~FileSink() override
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    void log(const LogRecord& record) override
    {
        if (!logFile)
        {
            return;
        }

        logFile << LogFormatter::FormatFile(record) << std::endl;
    }

   private:
    std::ofstream logFile;
};

class Logger
{
   public:
    void addSink(std::shared_ptr<LogSink> sink) { sinks.push_back(sink); }

    void log(LogLevel level,
             const std::string& message,
             const char* file = nullptr,
             int line = 0,
             const char* function = nullptr)
    {
        std::lock_guard<std::mutex> lock(logMutex);

        LogRecord record{level, message, file, line, function, std::chrono::system_clock::now()};

        for (auto& sink : sinks)
        {
            if (sink != nullptr && sink->ShouldLog(level))
            {
                sink->log(record);
            }
        }
    }

    void debug(const std::string& message, const char* file, int line, const char* function)
    {
        log(LogLevel::DEBUG, message, file, line, function);
    }

    void info(const std::string& message, const char* file, int line, const char* function)
    {
        log(LogLevel::INFO, message, file, line, function);
    }

    void warn(const std::string& message, const char* file, int line, const char* function)
    {
        log(LogLevel::WARNING, message, file, line, function);
    }

    void error(const std::string& message, const char* file, int line, const char* function)
    {
        log(LogLevel::ERROR, message, file, line, function);
    }

   private:
    std::vector<std::shared_ptr<LogSink>> sinks;
    std::mutex logMutex;
};

class LoggerRegistry
{
   public:
    static LoggerRegistry& getInstance()
    {
        static LoggerRegistry instance;
        return instance;
    }

    std::shared_ptr<Logger> getLogger(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(registryMutex);

        auto logger = loggers.find(name);

        if (logger != loggers.end())
        {
            return logger->second;
        }

        return defaultLogger;
    }

    void setDefaultLogger(std::shared_ptr<Logger> logger) { defaultLogger = logger; }

    void registerLogger(const std::string& name, std::shared_ptr<Logger> logger)
    {
        std::lock_guard<std::mutex> lock(registryMutex);
        loggers[name] = logger;
    }

   private:
    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;
    std::shared_ptr<Logger> defaultLogger = std::make_shared<Logger>();
    std::mutex registryMutex;
};
}  // namespace Engine

#define LOG_DEBUG(message) \
    ::Engine::LoggerRegistry::getInstance().getLogger("global")->debug(message, __FILE__, __LINE__, __func__)

#define LOG_INFO(message) \
    ::Engine::LoggerRegistry::getInstance().getLogger("global")->info(message, __FILE__, __LINE__, __func__)

#define LOG_WARN(message) \
    ::Engine::LoggerRegistry::getInstance().getLogger("global")->warn(message, __FILE__, __LINE__, __func__)

#define LOG_ERROR(message) \
    ::Engine::LoggerRegistry::getInstance().getLogger("global")->error(message, __FILE__, __LINE__, __func__)
