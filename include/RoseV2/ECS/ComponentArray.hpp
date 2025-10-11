#pragma once

#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/Core/Assert.hpp"

#include "RoseV2/pch.hpp"

namespace RoseV2::ECS {

class iComponentArray {
public:
    virtual ~iComponentArray() = default;

    virtual void remove(Entity e) = 0;
    virtual bool has(Entity e) const = 0;
    virtual size_t size() const = 0;
    virtual Entity getOwner(size_t index) const = 0;
    virtual const std::unordered_map<Entity, uint32_t, EntityHash>& getEntityToIndexMap() const = 0;
};

template<typename T>
class ComponentArray : public iComponentArray {
public:
    void add(Entity e, const T& component);
    void remove(Entity e) override;

    T& get(Entity e);
    const T& get(Entity e) const;
        
    bool has(Entity e) const override;
    size_t size() const override;
    Entity getOwner(size_t index) const override;

    const std::unordered_map<Entity, uint32_t, EntityHash>& getEntityToIndexMap() const override;
private:
    std::vector<T> m_Components;
    std::vector<Entity> m_Owners;
    std::unordered_map<Entity, uint32_t, EntityHash> m_EntityToIndexMap;
};

} // namespace RoseV2::ECS
