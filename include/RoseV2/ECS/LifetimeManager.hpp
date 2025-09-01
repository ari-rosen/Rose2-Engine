#pragma once

#include "RoseV2/ECS/Entity.hpp"

#include "RoseV2/pch.hpp"

namespace RoseV2::ECS {

class LifetimeManager {
public:
    LifetimeManager();

    Entity createEntity();
    void destroyEntity(Entity e);

    bool isEntityAlive(Entity e) const;
private:
    Entity m_NextID;
    std::queue<Entity> m_RecycledIDs;
    std::unordered_map<Entity, bool> m_AliveEntities;
};

} // namespace RoesV2::ECS
