#pragma once

#include "RoseV2/pch.hpp"

namespace RoseV2::Core {

enum class LogLevel {
    Info, Warning, Error, Fatal
};

class Logger {
public:
    static void log(LogLevel level, const std::string &message);
private:
    static std::string logLevelToString(LogLevel level);
    static std::string getTimestamp();
};

} // namespace RoseV2::Core
