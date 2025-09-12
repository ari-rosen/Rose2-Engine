#pragma once

#include "RoseV2/Graphics/UniformBuffer.hpp"

namespace RoseV2::Platform::OpenGL {

class OpenGLUniformBuffer : public Graphics::UniformBuffer {
public:
    OpenGLUniformBuffer(uint32_t size, uint32_t binding);
    ~OpenGLUniformBuffer();

    void setData(const void *data, uint32_t size, uint32_t offset = 0) override;
private:
    uint32_t m_UBO;
};

}
