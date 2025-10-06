#include "RoseV2/Core/Logger.hpp"

namespace RoseV2::Core {

void Logger::log(LogLevel level, const std::string &message) {
    std::string prefix = "[" + logLevelToString(level) + "] ";
    std::string timestamp = getTimestamp();

    (*s_OutputStream) << "[RoseV2] " << timestamp << " | " << prefix << message << std::endl;

    if (level == LogLevel::Fatal) {
        std::cerr << "Fatal error occurred, aborting." << std::endl;
        std::abort();
    }
} 

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error:   return "ERROR";
        case LogLevel::Fatal:   return "FATAL";
        default: return "UNKNOWN";
    }
}

std::string Logger::getTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buffer);
}

} // namespace RoseV2::Core
