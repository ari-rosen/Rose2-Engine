#pragma once

namespace RoseV2::Graphics {

class Renderer {
public:
    virtual ~Renderer() = default;

    static std::unique_ptr<Renderer> create();
    
    virtual void init() = 0;
    virtual void beginScene() = 0;
    virtual void submitSprite() = 0;
    virtual void endScene() = 0;
};

} // RoseV2::Graphics
