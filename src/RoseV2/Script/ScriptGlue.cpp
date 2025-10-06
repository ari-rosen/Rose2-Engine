#include "RoseV2/Script/ScriptGlue.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/Scene/Scene.hpp"
#include "RoseV2/Script/LuaEngine.hpp"

namespace RoseV2::Script {

static LuaEntity* checkEntity(lua_State* L, int index) {
    return (LuaEntity*)luaL_checkudata(L, index, "EntityMeta");
}

void ScriptGlue::registerLuaFunctions() {
    lua_State *L = LuaEngine::getLuaState();

    lua_pushcfunction(L, lua_Print);
    lua_setglobal(L, "print");
    
    // Scene module
    lua_newtable(L);
    lua_pushcfunction(L, lua_SceneCreateEntity);
    lua_setfield(L, -2, "createEntity");
    lua_setglobal(L, "Scene");
    
    // Entity metatable
    luaL_newmetatable(L, "EntityMeta");
    
    lua_pushcfunction(L, lua_EntitySetTag);
    lua_setfield(L, -2, "setTag");
    
    lua_pushcfunction(L, lua_EntitySetPosition);
    lua_setfield(L, -2, "setPosition");
    
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    lua_pop(L, 1);
}

int ScriptGlue::lua_Print(lua_State *L) {
    int n = lua_gettop(L);
    std::stringstream ss;

    for (int i = 1; i <= n; i++) {
        if (lua_isstring(L, i)) {
            ss << lua_tostring(L, i);
        } else {
            ss << lua_typename(L, lua_type(L, i));
        }

        if (i < n) ss << "\t";
    }

    ss << "\n";
    
    Core::Logger::log(Core::LogLevel::Info, ss.str().c_str());

    return 0;
}

int ScriptGlue::lua_SceneCreateEntity(lua_State *L) {
    auto scene = LuaEngine::getActiveScene();
    ECS::Entity e = scene->getRegistry().createEntity();
    scene->getRegistry().addComponent(e, ECS::TransformComponent{});
    scene->getRegistry().addComponent(e, ECS::TagComponent{"Entity"});

    auto* luaEnt = (LuaEntity*)lua_newuserdata(L, sizeof(LuaEntity));
    luaEnt->Entity = e;
    luaEnt->Registry = &scene->getRegistry();

    luaL_getmetatable(L, "EntityMeta");
    lua_setmetatable(L, -2);

    return 1;
}

int ScriptGlue::lua_SceneDestroyEntity(lua_State *L) {
    
    return 0;
}

int ScriptGlue::lua_SceneIsEntityAlive(lua_State *L) {

    return 1;
}

int ScriptGlue::lua_EntitySetTag(lua_State* L) {
    auto* luaEnt = checkEntity(L, 1);
    const char* tag = luaL_checkstring(L, 2);

    auto& registry = *luaEnt->Registry;
    if (!registry.hasComponent<RoseV2::ECS::TagComponent>(luaEnt->Entity)) {
        registry.addComponent(luaEnt->Entity, RoseV2::ECS::TagComponent{tag});
    } else {
        auto& tc = registry.getComponent<RoseV2::ECS::TagComponent>(luaEnt->Entity);
        tc.Tag = tag;
    }

    return 0;
}

int ScriptGlue::lua_EntitySetPosition(lua_State* L) {
    auto* luaEnt = checkEntity(L, 1);
    float x = (float)luaL_checknumber(L, 2);
    float y = (float)luaL_checknumber(L, 3);
    float z = (float)luaL_checknumber(L, 4);

    auto& registry = *luaEnt->Registry;
    auto& transform = registry.getComponent<RoseV2::ECS::TransformComponent>(luaEnt->Entity);

    transform.Translation = {x, y, z};

    return 0;
}


}
