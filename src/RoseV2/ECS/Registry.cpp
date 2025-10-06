#include "RoseV2/ECS/Registry.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/ECS/Entity.hpp"

namespace RoseV2::ECS {

Registry::Registry() {
    m_ComponentManager.getArray<TransformComponent>();
    m_ComponentManager.getArray<SpriteComponent>();
    m_ComponentManager.getArray<TagComponent>();
    m_ComponentManager.getArray<CameraComponent>();
    m_ComponentManager.getArray<ScriptComponent>();
    m_ComponentManager.getArray<UUIDComponent>();
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
