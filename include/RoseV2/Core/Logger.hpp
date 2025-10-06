#pragma once

#include "RoseV2/pch.hpp"

namespace RoseV2::Core {

enum class LogLevel {
    Info, Warning, Error, Fatal
};

class Logger {
public:
    static void log(LogLevel level, const std::string &message);
    static void setOutputStream(std::ostream& stream) { s_OutputStream = &stream; }
private:
    static std::string logLevelToString(LogLevel level);
    static std::string getTimestamp();

    inline static std::ostream* s_OutputStream = &std::cout;
};

} // namespace RoseV2::Core
