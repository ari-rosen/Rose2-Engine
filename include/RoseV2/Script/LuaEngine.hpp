#pragma once

#include "RoseV2/Core/Timestep.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/Scene/Scene.hpp"
#include "RoseV2/Script/ScriptFieldRegistry.hpp"
#include "RoseV2/Script/RuntimeEntityTables.hpp"

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
    static inline ScriptFieldRegistry& getFieldRegistry() { return s_FieldRegistry; }
    static inline RuntimeEntityTables& getRuntimeEntityTables() { return s_RuntimeEntityTables; }
    
    static int bytecodeWriter(lua_State *L, const void *p, size_t size, void *data); // for AssetImporter
private:
    static lua_State* s_LuaState;
    static Scene::Scene* s_ActiveScene;

    static ScriptFieldRegistry s_FieldRegistry;
    static RuntimeEntityTables s_RuntimeEntityTables;
};

};

extern "C" int luaBytecodeWriterWrapper(lua_State *L, const void *p, size_t size, void *data); // for AssetImporter
