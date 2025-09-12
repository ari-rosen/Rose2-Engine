#pragma once

#include <glm/glm.hpp>
#include "RoseV2/pch.hpp"

namespace RoseV2::ECS {

struct TagComponent {
    std::string Tag;
};

struct TransformComponent {
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
};

struct SpriteComponent {
    glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
};

} // namespace RoseV2::ECS
