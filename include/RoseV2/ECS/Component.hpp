#pragma once

#include <glm/glm.hpp>
#include "RoseV2/Asset/Asset.hpp"
#include "RoseV2/Core/Core.hpp"
#include "RoseV2/Core/UUID.hpp"
#include "RoseV2/Graphics/Camera.hpp"
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
    Asset::AssetHandle TextureHandle = 0;
};

struct ROSE_V2_API CameraComponent {
    float Zoom = 1.0f;
    float Left = -1.0f;
    float Right = 1.0f;
    float Bottom = -1.0f;
    float Top = 1.0f;
    bool Primary = true;
};

struct ROSE_V2_API ScriptComponent {
    Asset::AssetHandle ScriptHandle = 0;
};

struct ROSE_V2_API UUIDComponent {
    Core::UUID ID;
};

using AllComponents = std::tuple<
    TransformComponent, SpriteComponent, TagComponent,
    CameraComponent, ScriptComponent, UUIDComponent
>;

} // namespace RoseV2::ECS
