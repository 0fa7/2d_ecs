#pragma once

#include <string>
#include <vector>

#define LOG_COLOR_RESET   "\033[0m"
#define LOG_COLOR_RED     "\033[31m"      // Foreground Red
#define LOG_COLOR_GREEN   "\033[32m"      // Foreground Green
#define LOG_COLOR_YELLOW  "\033[33m"      // Foreground Yellow
#define LOG_COLOR_BLUE    "\033[34m"      // Foreground Blue
#define LOG_COLOR_BG_RED  "\033[41m"      // Background Red

enum class LogType : int
{
    LOG_INFO = 0,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry
{
    LogType m_type;
    std::string m_message;
};

class Logger
{
public:
    Logger();
    ~Logger();
    static void Info(const std::string& message);
    static void Err(const std::string& message);
    static std::string CurrentDateTimeToString();

    static std::vector<LogEntry> m_log_entries;
};