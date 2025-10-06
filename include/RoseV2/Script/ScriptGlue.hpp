#pragma once

#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/ECS/Registry.hpp"
#include "RoseV2/Scene/Scene.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"

namespace RoseV2::Script {

struct LuaEntity {
    ECS::Entity Entity;
    ECS::Registry *Registry;
};

class ScriptGlue {
public:
public:
    static void registerLuaFunctions();
private:
    static int lua_Print(lua_State *L);

    static int lua_SceneCreateEntity(lua_State *L);
    static int lua_SceneDestroyEntity(lua_State *L);
    static int lua_SceneIsEntityAlive(lua_State *L);
    
    static int lua_EntitySetTag(lua_State *L);
    static int lua_EntitySetPosition(lua_State *L);
    static int lua_EntityAddSprite(lua_State *L);
};

}
