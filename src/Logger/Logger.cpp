#include "Logger.hpp"

#include <chrono>
#include <format>
#include <iostream>
#include <syncstream>

std::vector<LogEntry> Logger::m_log_entries;

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::Info(const std::string& message)
{
    LogEntry log_entry;
    log_entry.m_type = LogType::LOG_INFO;
    log_entry.m_message = "INFO: [" + 
        CurrentDateTimeToString() + 
        "] " +
        message;

    std::osyncstream(std::cout) << 
        LOG_COLOR_GREEN << 
        log_entry.m_message << 
        std::endl << 
        LOG_COLOR_RESET;
    
    m_log_entries.push_back(log_entry);
}

void Logger::Err(const std::string& message)
{
    LogEntry log_entry;
    log_entry.m_type = LogType::LOG_ERROR;
    log_entry.m_message = "ERR: [" + 
        CurrentDateTimeToString() + 
        "] " +
        message;
    
    std::osyncstream(std::cout) << 
        LOG_COLOR_RED << 
        log_entry.m_message << 
        std::endl << 
        LOG_COLOR_RESET;

    
    m_log_entries.push_back(log_entry);
}

std::string Logger::CurrentDateTimeToString()
{
    auto now = std::chrono::system_clock::now();
    return std::format("{}", std::chrono::current_zone()->to_local(now));
}