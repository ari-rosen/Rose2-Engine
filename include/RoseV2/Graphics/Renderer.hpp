#pragma once

#include "RoseV2/Graphics/Shader.hpp"

namespace RoseV2::Graphics {

class Renderer {
public:
    static void init();
    static void beginScene();
    static void submitSprite();
    static void endScene();

private:
    static inline std::unordered_map<std::string, std::shared_ptr<Shader>> s_CoreShaders;
};

} // RoseV2::Graphics
