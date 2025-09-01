#pragma once

#include "RoseV2/ECS/Entity.hpp"

namespace RoseV2::ECS {

class iComponentArray {
    virtual ~iComponentArray() = default;
};

template<typename T>
class ComponentArray : iComponentArray {
public:
    void add(Entity e, const T& component);
    void remove(Entity e);
    bool has(Entity e) const;
    T& get(Entity e);
private:
    std::vector<T> m_Components;
    std::vector<Entity> m_Owners;
    std::unordered_map<Entity, size_t> m_EntityToIndexMap;
};

} // namespace RoseV2::ECS
