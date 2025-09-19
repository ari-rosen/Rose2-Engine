#include "RoseV2/ECS/LifetimeManager.hpp"
#include "RoseV2/Core/Assert.hpp"

namespace RoseV2::ECS {

LifetimeManager::LifetimeManager()
    : m_NextID(0)
{}

Entity LifetimeManager::createEntity() {
    Entity e;
    if (!m_RecycledIDs.empty()) {
        e.id = m_RecycledIDs.front();
        m_RecycledIDs.pop();

        auto it = m_IdToAliveGeneration.find(e.id);
        e.generation = (it != m_IdToAliveGeneration.end()) ? it->second + 1 : 0;
    }
    else {
        e.id = m_NextID++;
        e.generation = 0;
    }

    m_IdToAliveGeneration[e.id] = e.generation;
    
    return e;
}

void LifetimeManager::destroyEntity(Entity e) {
    ROSEV2_ASSERT(isEntityAlive(e), "Attempt to destroy an Entity that doesn't exist");

    auto genIt = m_IdToAliveGeneration.find(e.id);
    if (genIt != m_IdToAliveGeneration.end()) {
        genIt->second++;
    }

    m_RecycledIDs.push(e.id);
}

bool LifetimeManager::isEntityAlive(Entity e) const {
    auto it = m_IdToAliveGeneration.find(e.id);
    return it != m_IdToAliveGeneration.end() && it->second == e.generation;
}

}
