#pragma once

#include "RoseV2/Graphics/RendererAPI.hpp"

namespace RoseV2::Platform::OpenGL {

class OpenGLRendererAPI : public Graphics::RendererAPI {
public:
    void init() override;
    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void setClearColor(float r, float g, float b) override;
    void clear() override;

};

}
