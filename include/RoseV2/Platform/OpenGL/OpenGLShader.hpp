#pragma once

#include "RoseV2/Graphics/Shader.hpp"

#include <glad/glad.h>

namespace RoseV2::Platform::OpenGL {

class OpenGLShader : public Graphics::Shader {
public:
    OpenGLShader(const std::filesystem::path& vertexPath,
                 const std::filesystem::path& fragmentPath);
    ~OpenGLShader() override;

    void bind() const override;
    void unbind() const override;

    void setUniformInt(const std::string& name, int value) override;
    void setUniformFloat(const std::string& name, float value) override;
    void setUniformVec2(const std::string& name, const glm::vec2& value) override;
    void setUniformVec3(const std::string& name, const glm::vec3& value) override;
    void setUniformVec4(const std::string& name, const glm::vec4& value) override;
    void setUniformMat3(const std::string& name, const glm::mat3& value) override;
    void setUniformMat4(const std::string& name, const glm::mat4& value) override;

    void setUniformIntArray(const std::string &name, int *values, uint32_t count) override;

    void setUniformBinding(const std::string &name, const uint32_t binding) override;
private:
    int getUniformLocation(const std::string& name) const;
    uint32_t compileShader(GLenum type, const std::string& source);
    static std::string readFile(const std::filesystem::path& path);
private:
    uint32_t m_ProgramID;

    mutable std::unordered_map<std::string, int> m_uniformCache;
};

}
