// @file Logger.h

#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

namespace Engine
{
enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
   public:
    static Logger& Instance()
    {
        static Logger logger;
        return logger;
    }

    void OpenFile(const std::string& filename)
    {
        std::filesystem::path path(filename);

        if (path.has_parent_path())
        {
            std::filesystem::create_directories(path.parent_path());
        }

        file.open(filename, std::ios::out);
    }

    void Write(LogLevel level, const std::string& message, const char* sourceFile, int line,
               const char* function)
    {
        std::lock_guard<std::mutex> lock(mutex);
        const auto time = std::chrono::system_clock::now();

        if (level != LogLevel::DEBUG)
        {
            std::cout << "[" << FormatTime(time, false) << "] [" << LevelName(level) << "] "
                      << message << std::endl;
        }

        if (file)
        {
            file << "[" << FormatTime(time, true) << "] [" << LevelName(level) << "] "
                 << ShortFileName(sourceFile) << ":" << line;

            if (function != nullptr)
            {
                file << " (" << function << ")";
            }

            file << " | " << message << std::endl;
        }
    }

   private:
    Logger() = default;

    static const char* LevelName(LogLevel level)
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

    static std::string FormatTime(std::chrono::system_clock::time_point time, bool includeDate)
    {
        const auto timeValue = std::chrono::system_clock::to_time_t(time);
        const auto milliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()) % 1000;

        std::tm localTime = {};
        localtime_s(&localTime, &timeValue);

        std::ostringstream stream;

        if (includeDate)
        {
            stream << std::put_time(&localTime, "%Y-%m-%d ");
        }

        stream << std::put_time(&localTime, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3)
               << milliseconds.count();
        return stream.str();
    }

    static std::string ShortFileName(const char* sourceFile)
    {
        return sourceFile != nullptr ? std::filesystem::path(sourceFile).filename().string()
                                     : "unknown";
    }

    std::ofstream file;
    std::mutex mutex;
};
}  // namespace Engine

#define LOG_DEBUG(message)                                                                     \
    ::Engine::Logger::Instance().Write(::Engine::LogLevel::DEBUG, message, __FILE__, __LINE__, \
                                       __func__)

#define LOG_INFO(message)                                                                     \
    ::Engine::Logger::Instance().Write(::Engine::LogLevel::INFO, message, __FILE__, __LINE__, \
                                       __func__)

#define LOG_WARN(message)                                                                        \
    ::Engine::Logger::Instance().Write(::Engine::LogLevel::WARNING, message, __FILE__, __LINE__, \
                                       __func__)

#define LOG_ERROR(message)                                                                     \
    ::Engine::Logger::Instance().Write(::Engine::LogLevel::ERROR, message, __FILE__, __LINE__, \
                                       __func__)
