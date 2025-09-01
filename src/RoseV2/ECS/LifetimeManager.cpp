#include "RoseV2/ECS/LifetimeManager.hpp"

#include "RoseV2/Core/Assert.hpp"

namespace RoseV2::ECS {

LifetimeManager::LifetimeManager()
    : m_NextID(0)
{}

Entity LifetimeManager::createEntity() {
    if (!m_RecycledIDs.empty()) {
        Entity e = m_RecycledIDs.front();
        m_RecycledIDs.pop();
        m_AliveEntities[e] = true;
        return e;
    }
    else {
        Entity e = m_NextID++;
        m_AliveEntities[e] = true;
        return e;
    }
}

void LifetimeManager::destroyEntity(Entity e) {
    ROSEV2_ASSERT(isEntityAlive(e), "Attempt to destroy an Entity that doesn't exist");
    m_AliveEntities.erase(e);
    m_RecycledIDs.push(e);
}

bool LifetimeManager::isEntityAlive(Entity e) const {
    auto it = m_AliveEntities.find(e);
    if (it != m_AliveEntities.end()) {
        return it->second;
    }
    return false;
}

} // namespace RoseV2::ECS
