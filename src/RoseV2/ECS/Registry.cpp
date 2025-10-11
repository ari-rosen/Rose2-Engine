#include "RoseV2/ECS/Registry.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/ECS/Entity.hpp"

namespace RoseV2::ECS {

Registry::Registry() {
    auto registerComponents = [this](auto &&... components) {
        (m_ComponentManager.registerComponent<std::decay_t<decltype(components)>>(), ...);
    };
    std::apply(registerComponents, AllComponents{});
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
