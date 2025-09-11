#pragma once

#include "RoseV2/ECS/ComponentManager.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/ECS/LifetimeManager.hpp"

namespace RoseV2::ECS {

class Registry {
public:
    Entity createEntity();
    void destroyEntity(Entity e);

    template<typename T>
    void addComponent(Entity e, const T& component) {
        m_ComponentManager.addComponent(e, component);
    }

    template<typename T>
    void removeComponent(Entity e) {
        m_ComponentManager.removeComponent<T>(e);
    }

    template<typename T>
    bool hasComponent(Entity e) {
        return m_ComponentManager.hasComponent<T>(e);
    }

private:
    LifetimeManager m_LifetimeManager;
    ComponentManager m_ComponentManager;
};

}
