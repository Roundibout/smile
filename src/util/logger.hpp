#pragma once

#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <iomanip>
#include <sstream>

enum class LogLevel {
    Info,
    Warning,
    Error
};

struct LogEntry {
    LogLevel level;
    std::string message;
    std::string timestamp;
};


inline std::string getCurrentTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::tm localTime{};
    #ifdef _WIN32
        localtime_s(&localTime, &time);
    #else
        localtime_r(&time, &localTime);
    #endif

    std::ostringstream oss;
    oss << '['
        << std::setfill('0') << std::setw(2) << localTime.tm_hour << ':'
        << std::setw(2) << localTime.tm_min << ':'
        << std::setw(2) << localTime.tm_sec << '.'
        << std::setw(3) << ms.count()
        << "] ";
    return oss.str();
}

namespace Logger {
    inline std::vector<LogEntry> logs;
    // Callback functions for new log events
    inline std::vector<std::function<void(const LogEntry&)>> callbacks;

    inline void log(LogLevel level, const std::string& message) {
        if (logs.size() > 500) {
            logs.erase(logs.begin());
        }
        LogEntry entry{level, message, getCurrentTimestamp()};
        logs.push_back(entry);
        for (auto& callback : callbacks) callback(entry);
    }

    inline void print(const std::string& message) {log(LogLevel::Info, message);}
    inline void warn(const std::string& message) {log(LogLevel::Warning, message);}
    inline void error(const std::string& message) {log(LogLevel::Error, message);}

    inline void append(const std::string& message) {
        if (!logs.empty()) {
            logs.back().message += message;
            for (auto& callback : callbacks) callback(logs.back());
        }
    }

    // Allow other systems to subscribe to new logs (console)
    inline void addCallback(std::function<void(const LogEntry&)> cb) {
        callbacks.push_back(cb);
    }

    inline const std::vector<LogEntry>& getAllLogs() {return logs;}
}