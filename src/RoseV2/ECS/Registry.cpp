#include "RoseV2/ECS/Registry.hpp"
#include "RoseV2/ECS/Entity.hpp"

namespace RoseV2::ECS {

Entity Registry::createEntity() {
    return m_LifetimeManager.createEntity();
}

void Registry::destroyEntity(Entity e) {
    m_LifetimeManager.destroyEntity(e);
    m_ComponentManager.entityDestroyed(e);
}

const std::vector<Entity> &Registry::getAliveEntities() const {
    return m_LifetimeManager.getEntities();
}

}
