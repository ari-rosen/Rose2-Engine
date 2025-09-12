
#include "RoseV2/Platform/OpenGL/OpenGLShader.hpp"
#include "RoseV2/Platform/OpenGL/OpenGLError.hpp"
#include "RoseV2/Core/Logger.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace RoseV2::Platform::OpenGL {

std::string OpenGLShader::readFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        Core::Logger::log(Core::LogLevel::Error, "Failed to open shader file: " + path.string());
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

uint32_t OpenGLShader::compileShader(GLenum type, const std::string& source) {
    uint32_t shader = 0;
    GL_CALL(shader = glCreateShader(type));
    const char* src = source.c_str();
    GL_CALL(glShaderSource(shader, 1, &src, nullptr));
    GL_CALL(glCompileShader(shader));

    int success;
    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success) {
        char infoLog[512];
        GL_CALL(glGetShaderInfoLog(shader, 512, nullptr, infoLog));
        Core::Logger::log(Core::LogLevel::Error,
            "Shader compilation failed (" + std::string(type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") + "): " + infoLog);
    }

    return shader;
}

OpenGLShader::OpenGLShader(const std::filesystem::path& vertexPath,
                           const std::filesystem::path& fragmentPath) {
    std::string vertexSrc = readFile(vertexPath);
    std::string fragmentSrc = readFile(fragmentPath);

    if (vertexSrc.empty() || fragmentSrc.empty()) {
        Core::Logger::log(Core::LogLevel::Error, "Shader sources are empty. Failed to load.");
        m_ProgramID = 0;
        return;
    }

    uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    GL_CALL(m_ProgramID = glCreateProgram());
    GL_CALL(glAttachShader(m_ProgramID, vertexShader));
    GL_CALL(glAttachShader(m_ProgramID, fragmentShader));
    GL_CALL(glLinkProgram(m_ProgramID));

    int success;
    GL_CALL(glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success));
    if (!success) {
        char infoLog[512];
        GL_CALL(glGetProgramInfoLog(m_ProgramID, 512, nullptr, infoLog));
        Core::Logger::log(Core::LogLevel::Error,
            "Shader program linking failed: " + std::string(infoLog));
    }

    GL_CALL(glDeleteShader(vertexShader));
    GL_CALL(glDeleteShader(fragmentShader));
}

OpenGLShader::~OpenGLShader() {
    if (m_ProgramID != 0) {
        glDeleteProgram(m_ProgramID);
    }
}

void OpenGLShader::bind() const {
    glUseProgram(m_ProgramID);
}

void OpenGLShader::unbind() const {
    glUseProgram(0);
}

int OpenGLShader::getUniformLocation(const std::string& name) const {
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end())
        return it->second;

    int location = glGetUniformLocation(m_ProgramID, name.c_str());
    if (location == -1) {
        Core::Logger::log(Core::LogLevel::Error, "Uniform not found: " + name);
    }

    m_uniformCache[name] = location;
    return location;
}

void OpenGLShader::setUniformInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void OpenGLShader::setUniformFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void OpenGLShader::setUniformVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void OpenGLShader::setUniformVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void OpenGLShader::setUniformVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void OpenGLShader::setUniformMat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::setUniformMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::setUniformBinding(const std::string &name, const uint32_t binding) {
    GLuint blockIndex = glGetUniformBlockIndex(m_ProgramID, name.c_str());
    if (blockIndex == GL_INVALID_INDEX) {
        Core::Logger::log(Core::LogLevel::Warning, "Failure to bind uniform block: Uniform block " + name + " not found");
        return;
    }
    glUniformBlockBinding(m_ProgramID, blockIndex, binding);   
}


}
