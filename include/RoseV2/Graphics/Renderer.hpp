#pragma once

#include "RoseV2/Core/Core.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/Graphics/Camera.hpp"

namespace RoseV2::Graphics {

class ROSE_V2_API Renderer {
public:
    static void init();
    static void shutdown();

    static void beginScene(const Camera &camera);

    static void submitQuad(const glm::mat4 &transform, const glm::vec4 &color, uint32_t texIndex);
    static void submitSprite(const ECS::TransformComponent &transform, const ECS::SpriteComponent &sprite);

    static void endScene();
private:
    static void beginBatch();
    static void flushBatch();
};

} // RoseV2::Graphics
