#pragma once

#include "RoseV2/Graphics/Renderer.hpp"

namespace RoseV2::Graphics {

class OpenGLRenderer : public Renderer {
public:
    void init() override;
    void beginScene() override;
    void submitSprite() override;
    void renderScene() override;
    void endScene() override;
};

} // namespace RoseV2::Graphics
