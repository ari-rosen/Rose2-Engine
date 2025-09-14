#include "RoseV2/ECS/LifetimeManager.hpp"

#include "RoseV2/Core/Assert.hpp"

namespace RoseV2::ECS {

LifetimeManager::LifetimeManager()
    : m_NextID(0)
{}

Entity LifetimeManager::createEntity() {
    Entity e;
    if (!m_RecycledIDs.empty()) {
        e = m_RecycledIDs.front();
        m_RecycledIDs.pop();
    }
    else {
        e = m_NextID++;
    }

    m_AliveEntities[e] = true;
    m_AllEntities.push_back(e);

    return e;
}

void LifetimeManager::destroyEntity(Entity e) {
    ROSEV2_ASSERT(isEntityAlive(e), "Attempt to destroy an Entity that doesn't exist");
    m_AliveEntities.erase(e);
    m_RecycledIDs.push(e);

    auto it = std::find(m_AllEntities.begin(), m_AllEntities.end(), e);
    if (it != m_AllEntities.end()) {
        m_AllEntities.erase(it);
    }
}

bool LifetimeManager::isEntityAlive(Entity e) const {
    auto it = m_AliveEntities.find(e);
    if (it != m_AliveEntities.end()) {
        return it->second;
    }
    return false;
}

} // namespace RoseV2::ECS
