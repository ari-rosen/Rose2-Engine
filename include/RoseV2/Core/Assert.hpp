#pragma once

#include "RoseV2/Core/Logger.hpp"

inline void RoseV2AssertImpl(bool condition, const char* message,
                             const char* file, int line) {
    if (!condition) {
        RoseV2::Core::Logger::log(RoseV2::Core::LogLevel::Fatal, std::string("ASSERT FAILED: ") + message + " (" + file + ":" + std::to_string(line) + ")");
    }
}

#define ROSEV2_ASSERT(cond, msg) RoseV2AssertImpl((cond), (msg), __FILE__, __LINE__)


