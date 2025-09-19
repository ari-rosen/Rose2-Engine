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
    uint32_t m_NextID;
    std::queue<uint32_t> m_RecycledIDs;
    std::unordered_map<uint32_t, uint32_t> m_IdToAliveGeneration{};
};

} // namespace RoesV2::ECS
