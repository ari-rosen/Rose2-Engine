#pragma once

#include <glm/glm.hpp>
#include "RoseV2/Core/Core.hpp"
#include "RoseV2/pch.hpp"

namespace RoseV2::ECS {

struct ROSE_V2_API TagComponent {
    std::string Tag;
};

struct ROSE_V2_API TransformComponent {
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
};

struct ROSE_V2_API SpriteComponent {
    glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
};

} // namespace RoseV2::ECS
