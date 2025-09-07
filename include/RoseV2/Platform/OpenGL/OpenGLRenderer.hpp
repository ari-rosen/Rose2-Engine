#pragma once

#include "RoseV2/Graphics/Renderer.hpp"

namespace RoseV2::Platform::OpenGL {

class OpenGLRenderer : public Graphics::Renderer {
public:
    void init() override;
    void beginScene() override;
    void submitSprite() override;
    void endScene() override;
};

} // namespace RoseV2::Graphics
