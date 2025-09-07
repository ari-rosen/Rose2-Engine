#pragma once

#include "RoseV2/Graphics/GraphicsContext.hpp"

namespace RoseV2::Platform::OpenGL {

class OpenGLGraphicsContext : public Graphics::GraphicsContext {
public:
    void init() override;
};

} // namespace RoseV2::Platform::OpenGL
