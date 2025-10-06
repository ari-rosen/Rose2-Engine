#pragma once

#include "RoseV2/Core/Assert.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/ECS/ComponentArray.hpp"

#include "RoseV2/pch.hpp"

namespace RoseV2::ECS {

class ComponentManager {
public:
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
    bool hasComponent(Entity e) {
        return getArray<T>().has(e);
    }
    
    template<typename T>
    bool hasComponent(Entity e) const {
        auto it = m_ComponentArrays.find(std::type_index(typeid(T)));
        if (it == m_ComponentArrays.end()) return false;
        return static_cast<ComponentArray<T>*>(it->second.get())->has(e);
    }

    template<typename ...T>
    const iComponentArray* getSmallestArray() const {
        const iComponentArray* smallestArray = nullptr;
        std::initializer_list<const iComponentArray*> arrays = {&getArray<T>()...};

        for (const auto* array : arrays) {
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
