#include "RoseV2/Core/Logger.hpp"
#include <glad/glad.h>

inline void CheckGLError(const char* file, int line, const char* call) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (err) {
            case GL_INVALID_ENUM:                  error = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "GL_INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "GL_STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "GL_STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                               error = "UNKNOWN_ERROR"; break;
        }

        RoseV2::Core::Logger::log(RoseV2::Core::LogLevel::Error,
            "[OpenGL Error] " + error + 
            " | Call: " + std::string(call) +
            " | File: " + std::string(file) + 
            " | Line: " + std::to_string(line));
    }
}

#define GL_CALL(x) do { x; CheckGLError(__FILE__, __LINE__, #x); } while(0)
