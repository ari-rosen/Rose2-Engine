#pragma once

#include <glm/glm.hpp>

namespace RoseV2::Graphics {

class Shader {
public:
    virtual ~Shader() = default;

    static std::shared_ptr<Shader> create(std::filesystem::path vertPath, std::filesystem::path fragPath);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setUniformInt(const std::string& name, int value) = 0;
    virtual void setUniformFloat(const std::string& name, float value) = 0;
    virtual void setUniformVec2(const std::string& name, const glm::vec2& value) = 0;
    virtual void setUniformVec3(const std::string& name, const glm::vec3& value) = 0;
    virtual void setUniformVec4(const std::string& name, const glm::vec4& value) = 0;
    virtual void setUniformMat3(const std::string& name, const glm::mat3& value) = 0;
    virtual void setUniformMat4(const std::string& name, const glm::mat4& value) = 0;

};

} 
