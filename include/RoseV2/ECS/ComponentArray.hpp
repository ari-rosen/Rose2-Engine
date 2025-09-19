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
};

template<typename T>
class ComponentArray : public iComponentArray {
public:
    void add(Entity e, const T& component) {
        ROSEV2_ASSERT(!has(e), "Attempt to add the same component to an Entity twice");

        size_t index = m_Components.size();
        m_Components.push_back(component);
        m_Owners.push_back(e);
        m_EntityToIndexMap[e.id] = index;
    }
    
    void remove(Entity e) override {
        ROSEV2_ASSERT(has(e), "Attempt to remove a nonexistent component");
        ROSEV2_ASSERT(!m_Components.empty(), "Attempt to remove a component from an empty array");

        auto it = m_EntityToIndexMap.find(e.id);
        if (it == m_EntityToIndexMap.end()) return;

        size_t index = it->second;
        size_t last = m_Components.size() - 1;

        m_Components[index] = m_Components[last];
        m_Owners[index] = m_Owners[last];
        m_EntityToIndexMap[m_Owners[index].id] = index;

        m_Components.pop_back();
        m_Owners.pop_back();
        m_EntityToIndexMap.erase(it);
    }

    bool has(Entity e) const override {
        return m_EntityToIndexMap.count(e.id) > 0;    
    }

    T& get(Entity e) {
        ROSEV2_ASSERT(has(e), "Attempt to access a component that an Entity does not have");
        ROSEV2_ASSERT(m_Owners[m_EntityToIndexMap.at(e.id)] == e, "Component belongs to a dead entity");
        return m_Components[m_EntityToIndexMap.at(e.id)];
    }

    const std::unordered_map<uint32_t, uint32_t>& getEntityToIndexMap() const {
        return m_EntityToIndexMap;
    }
    
    Entity getOwner(size_t index) const {
        ROSEV2_ASSERT(index < m_Owners.size(), "Index out of bounds");
        return m_Owners[index];
    }

    size_t size() const override {
        return m_Components.size();
    }
private:
    std::vector<T> m_Components;
    std::vector<Entity> m_Owners;
    std::unordered_map<uint32_t, uint32_t> m_EntityToIndexMap;
};

} // namespace RoseV2::ECS
