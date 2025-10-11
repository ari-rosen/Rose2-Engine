#pragma once

#include "RoseV2/Core/Assert.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/ECS/ComponentArray.hpp"

#include "RoseV2/pch.hpp"

namespace RoseV2::ECS {

class ComponentManager {
public:
    template<typename T>
    void registerComponent() {
        auto typeID = std::type_index(typeid(T));
        ROSEV2_ASSERT(m_ComponentArrays.find(typeID) == m_ComponentArrays.end(), 
                           "Component type already registered!");
        m_ComponentArrays[typeID] = std::make_unique<ComponentArray<T>>();
    }

    template<typename T>
    void addComponent(Entity e, const T& component) {
        getArray<T>().add(e, component);
    }

    template<typename T>
    void removeComponent(Entity e) {
        getArray<T>().remove(e);
    }
   
    template<typename T>
    T& getComponent(Entity e) {
        return getArray<T>().get(e);
    }

    template<typename T>
    const T& getComponent(Entity e) const {
        return getArray<T>().get(e);
    }

    template<typename T>
    const ComponentArray<T>* findArray() const {
        auto it = m_ComponentArrays.find(std::type_index(typeid(T)));
        if (it == m_ComponentArrays.end()) {
            return nullptr;
        }
        return static_cast<const ComponentArray<T>*>(it->second.get());
    }
    
    template<typename T>
    bool hasComponent(Entity e) const {
        const ComponentArray<T>* array = findArray<T>();
        if (!array)
            return false; 
        
        return array->has(e);
    }

    template<typename ...T>
    const iComponentArray* getSmallestArray() const {
        const iComponentArray* smallestArray = nullptr;

        const iComponentArray* currentArrays[] = { findArray<T>()... };

        for (const auto* array : currentArrays) {
            if (!array) continue;

            if (!smallestArray || array->size() < smallestArray->size()) {
                smallestArray = array;
            }
        }

        return smallestArray;
    }

    void entityDestroyed(Entity e);

    template<typename First, typename... Rest>
    bool hasAllComponents(Entity e) {
        if (!hasComponent<First>(e)) {
            return false;
        }
        if constexpr (sizeof...(Rest) > 0) {
            return hasAllComponents<Rest...>(e);
        }
        return true;
    }

    template<typename T>
    ComponentArray<T>& getArray() {
        auto type = std::type_index(typeid(T));
        if (!m_ComponentArrays[type])
            m_ComponentArrays[type] = std::make_unique<ComponentArray<T>>();
        return *static_cast<ComponentArray<T>*>(m_ComponentArrays[type].get());
    }
    template<typename T>
    const ComponentArray<T>& getArray() const {
        auto type = std::type_index(typeid(T));
        return *static_cast<ComponentArray<T>*>(m_ComponentArrays.at(type).get());
    }
private:
    std::unordered_map<std::type_index, std::unique_ptr<iComponentArray>> m_ComponentArrays;
};

} // namespace RoseV2::ECS
