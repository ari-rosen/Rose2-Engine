#include "RoseV2/ECS/ComponentManager.hpp"

namespace RoseV2::ECS {

void ComponentManager::entityDestroyed(Entity e) {
    for (auto &[type, array] : m_ComponentArrays) {
        if (array->has(e)) {
            array->remove(e);
        }
    }
}
}
