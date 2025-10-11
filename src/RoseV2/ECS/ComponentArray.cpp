#include "RoseV2/ECS/ComponentArray.hpp"
#include "RoseV2/ECS/Component.hpp"

namespace RoseV2::ECS {

template<typename T>
void ComponentArray<T>::add(Entity e, const T &component) {
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

    if (index != last) {
        std::swap(m_Components[index], m_Components[last]);
        std::swap(m_Owners[index], m_Owners[last]);
        m_EntityToIndexMap[m_Owners[index]] = index;
    }

    m_Components.pop_back();
    m_Owners.pop_back();
    m_EntityToIndexMap.erase(it);
}

template<typename T>
T& ComponentArray<T>::get(Entity e) {
    ROSEV2_ASSERT(has(e), "Attempt to access a component that an Entity does not have");
    
    return m_Components[m_EntityToIndexMap.at(e)];
}

template<typename T>
const T& ComponentArray<T>::get(Entity e) const {
    ROSEV2_ASSERT(has(e), "Attempt to access a component that an Entity does not have");
    
    return m_Components[m_EntityToIndexMap.at(e)];
}

template<typename T>
bool ComponentArray<T>::has(Entity e) const {
    return m_EntityToIndexMap.count(e) > 0;    
}

template<typename T>
size_t ComponentArray<T>::size() const {
    return m_Components.size();
}

template<typename T>
Entity ComponentArray<T>::getOwner(size_t index) const {
    ROSEV2_ASSERT(index < m_Owners.size(), "Index out of bounds");
    return m_Owners[index];
}

template<typename T>
const std::unordered_map<Entity, uint32_t, EntityHash>& ComponentArray<T>::getEntityToIndexMap() const {
    return m_EntityToIndexMap;
}

template class ComponentArray<TagComponent>;
template class ComponentArray<TransformComponent>;
template class ComponentArray<SpriteComponent>;
template class ComponentArray<CameraComponent>;
template class ComponentArray<ScriptComponent>;
template class ComponentArray<UUIDComponent>;


}
