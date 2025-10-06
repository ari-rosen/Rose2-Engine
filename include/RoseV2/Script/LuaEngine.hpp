#pragma once

#include "RoseV2/Core/Timestep.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/Scene/Scene.hpp"

struct lua_State;

namespace RoseV2::Script {

class LuaEngine {
public:
    static void init();
    static void shutdown();

    static void onRuntimeStart(Scene::Scene *scene);
    static void onRuntimeStop();

    static void onStartEntity(ECS::Entity e);
    static void onUpdateEntity(ECS::Entity e, Core::Timestep ts);

    static inline lua_State* getLuaState() { return s_LuaState; }
    static inline Scene::Scene* getActiveScene() { return s_ActiveScene; }
private:
    static lua_State* s_LuaState;
    static Scene::Scene* s_ActiveScene;
};

};
