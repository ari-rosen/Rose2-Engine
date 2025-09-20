#pragma once

#include "RoseV2/Core/Assert.hpp"
#include "RoseV2/ECS/ComponentManager.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/ECS/LifetimeManager.hpp"
#include "RoseV2/ECS/View.hpp"

namespace RoseV2::ECS {

class Registry {
public:
    Registry();

    Entity createEntity();
    void destroyEntity(Entity e);
    bool isEntityAlive(Entity e);

    template<typename T>
    void addComponent(Entity e, const T& component) {
        ROSEV2_ASSERT(m_LifetimeManager.isEntityAlive(e), "Attempt to add a component to a destroyed entity");
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

    template<typename T>
    T& getComponent(Entity e) {
        return m_ComponentManager.getComponent<T>(e);
    }

    template<typename... Components>
    View<Components...> view() {
        return View<Components...>(&m_ComponentManager);
    }
private:
    LifetimeManager m_LifetimeManager;
    ComponentManager m_ComponentManager;
};

}
