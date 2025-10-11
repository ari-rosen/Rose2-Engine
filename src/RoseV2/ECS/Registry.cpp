#include "RoseV2/ECS/Registry.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/ECS/Entity.hpp"

namespace RoseV2::ECS {

Registry::Registry() {
    m_ComponentManager.registerComponent<TransformComponent>();
    m_ComponentManager.registerComponent<SpriteComponent>();
    m_ComponentManager.registerComponent<TagComponent>();
    m_ComponentManager.registerComponent<CameraComponent>();
    m_ComponentManager.registerComponent<ScriptComponent>();
    m_ComponentManager.registerComponent<UUIDComponent>();
}

Entity Registry::createEntity() {
    return m_LifetimeManager.createEntity();
}

void Registry::destroyEntity(Entity e) {
    m_ComponentManager.entityDestroyed(e);
    m_LifetimeManager.destroyEntity(e);
}

bool Registry::isEntityAlive(Entity e) {
    return m_LifetimeManager.isEntityAlive(e);
}

}
