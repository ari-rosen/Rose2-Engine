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

    void entityDestroyed(Entity e);
private:
    template<typename T>
    ComponentArray<T>& getArray() {
        auto type = std::type_index(typeid(T));
        if (!m_ComponentArrays[type])
            m_ComponentArrays[type] = std::make_unique<ComponentArray<T>>();
        return *static_cast<ComponentArray<T>*>(m_ComponentArrays[type].get());
    }
private:
    std::unordered_map<std::type_index, std::unique_ptr<iComponentArray>> m_ComponentArrays;
};

} // namespace RoseV2::ECS
