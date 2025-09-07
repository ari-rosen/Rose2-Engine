#pragma once

namespace RoseV2::Graphics {

class Renderer {
public:
    static void init();
    static void beginScene();
    static void submitSprite();
    static void endScene();
};

} // RoseV2::Graphics
