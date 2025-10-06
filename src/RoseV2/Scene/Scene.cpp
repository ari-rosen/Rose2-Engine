#include "RoseV2/Scene/Scene.hpp"
#include "RoseV2/Core/Timestep.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Script/ScriptGlue.hpp"

namespace RoseV2::Scene {

Scene::Scene()
    : m_IsRunning(false)
{}

void Scene::onRuntimeStart() {
    m_IsRunning = true;

    Script::LuaEngine::onRuntimeStart(this);

    auto view = m_Registry.view<ECS::ScriptComponent>();
    for (auto e : view) {
        Script::LuaEngine::onStartEntity(e);
    }
}

void Scene::onRuntimeStop() {
    m_IsRunning = false;
    Script::LuaEngine::onRuntimeStop();
}

void Scene::onRuntimeUpdate(Core::Timestep ts) {
    if (m_IsRunning) {
        auto view = m_Registry.view<ECS::ScriptComponent>();
        for (auto e : view) {
            Script::LuaEngine::onUpdateEntity(e, ts);
        }
    }
}

}
