#pragma once

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
    bool hasComponent(Entity e) {
        return getArray<T>().has(e);
    }


    template<typename... T>
    std::vector<Entity> view() {
        std::vector<Entity> entities;
        entities.reserve(256);

        std::initializer_list<std::type_index> types = { std::type_index(typeid(T))... };
        std::unique_ptr<iComponentArray>* smallestArray = nullptr;
        
        for (const auto& type : types) {
            auto it = m_ComponentArrays.find(type);
            if (it != m_ComponentArrays.end()) {
                if (!smallestArray || it->second->size() < (*smallestArray)->size()) {
                    smallestArray = &it->second;
                }
            }
        }

        if (!smallestArray) {
            return entities;
        }

        auto& firstArray = getArray<std::tuple_element_t<0, std::tuple<T...>>>();
        
        for (const auto& pair : firstArray.getEntityToIndexMap()) {
            Entity e = firstArray.getOwner(pair.second);
            
            if (hasAllComponents<T...>(e)) {
                entities.push_back(e);
            }
        }
        
        return entities;
    }

    void entityDestroyed(Entity e);
private:
    template<typename T>
    ComponentArray<T>& getArray() {
        auto type = std::type_index(typeid(T));
        if (!m_ComponentArrays[type])
            m_ComponentArrays[type] = std::make_unique<ComponentArray<T>>();
        return *static_cast<ComponentArray<T>*>(m_ComponentArrays[type].get());
    }

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
private:
    std::unordered_map<std::type_index, std::unique_ptr<iComponentArray>> m_ComponentArrays;
};

} // namespace RoseV2::ECS
