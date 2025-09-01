#include "RoseV2/ECS/ComponentArray.hpp"

#include "RoseV2/Core/Assert.hpp"

namespace RoseV2::ECS {

template<typename T>
void ComponentArray<T>::add(Entity e, const T& component) {
    ROSEV2_ASSERT(!has(e), "Attempt to add the same component to an Entity twice");

    size_t index = m_Components.size();
    m_Components.push_back(component);
    m_Owners.push_back(e);
    m_EntityToIndexMap[e] = index;
}

template<typename T>
void ComponentArray<T>::remove(Entity e) {
    ROSEV2_ASSERT(has(e), "Attempt to remove a nonexistent component");
    ROSEV2_ASSERT(!m_Components.empty(), "Attempt to remove a component from an empty array");

    auto it = m_EntityToIndexMap.find(e);
    if (it == m_EntityToIndexMap.end()) return;

    size_t index = it->second;
    size_t last = m_Components.size() - 1;

    m_Components[index] = m_Components[last];
    m_Owners[index] = m_Owners[last];
    m_EntityToIndexMap[m_Owners[index]] = index;

    m_Components.pop_back();
    m_Owners.pop_back();
    m_EntityToIndexMap.erase(it);
}

template<typename T>
bool ComponentArray<T>::has(Entity e) const {
    return m_EntityToIndexMap.count(e) > 0;    
}

template<typename T>
T& ComponentArray<T>::get(Entity e) {
    ROSEV2_ASSERT(has(e), "Attempt to access a component that an Entity does not have");

    return m_Components[m_EntityToIndexMap.at(e)];
}


} // namespace RoseV2::ECS
