#include "RoseV2/Script/ScriptInstance.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"
#include "RoseV2/Script/ScriptGlue.hpp"

namespace RoseV2::Script {

ScriptInstance::ScriptInstance(const std::string &name)
    : m_Name(name) {}

bool ScriptInstance::loadSource(const std::vector<std::byte> &bytecode) {
    if (m_TableRef.valid()) {
        m_TableRef.reset(); 
    }

    lua_State *L = LuaEngine::getLuaState();

    if (luaL_loadbuffer(
            L, 
            reinterpret_cast<const char *>(bytecode.data()),
            bytecode.size(),
            m_Name.c_str())
        != LUA_OK) 
    {
        Core::Logger::log(Core::LogLevel::Error, lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }

    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        lua_pop(L, 1);
        return false;
    }

    lua_getglobal(L, m_Name.c_str());
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return false;
    }

    m_TableRef = LuaRef(luaL_ref(L, LUA_REGISTRYINDEX));

    return true;
}

bool ScriptInstance::pushFunction(const char *functionName) {
    lua_State *L = LuaEngine::getLuaState();

    m_TableRef.push();
    lua_getfield(L, -1, functionName);
    lua_remove(L, -2);

    if (!lua_isfunction(L, -1))
        return false;

    return true;
}

bool ScriptInstance::pushEntity(ECS::Entity e) {
    lua_State* L = LuaEngine::getLuaState();
    auto* scene = LuaEngine::getActiveScene();

    if (!scene) return false;

    LuaEntity* luaEnt = (LuaEntity*)lua_newuserdata(L, sizeof(LuaEntity));
    luaEnt->Entity = e;
    luaEnt->Registry = &scene->getRegistry();

    luaL_getmetatable(L, "EntityMeta");
    lua_setmetatable(L, -2);

    return true;
}

void ScriptInstance::invokeOnStart(ECS::Entity e) {
    lua_State *L = LuaEngine::getLuaState();

    auto uuid = LuaEngine::getActiveScene()->getRegistry().getComponent<ECS::UUIDComponent>(e).ID;
    LuaRef& entityTable = LuaEngine::getRuntimeEntityTables().getEntityTable(uuid, m_Name);
    
    if (!pushFunction("onStart")) return; // start function
    entityTable.push(); // entity fields (self)
    if (!pushEntity(e)) return; // entity metatable
    
    // call function
    if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
        std::string errorFmt = "Lua OnStart error: " + std::string{lua_tostring(L, -1)} + "\n";
        Core::Logger::log(Core::LogLevel::Error, errorFmt);
        lua_pop(L, 1);
    }
}

void ScriptInstance::invokeOnUpdate(ECS::Entity e, Core::Timestep ts) {
    lua_State *L = LuaEngine::getLuaState();
    
    auto uuid = LuaEngine::getActiveScene()->getRegistry().getComponent<ECS::UUIDComponent>(e).ID;
    LuaRef& entityTable = LuaEngine::getRuntimeEntityTables().getEntityTable(uuid, m_Name);

    if (!pushFunction("onUpdate")) return; // onupdate function
    entityTable.push(); // entity fields
    if (!pushEntity(e)) return; // entity metatable
    lua_pushnumber(L, ts.getSeconds()); // deltatime
    
    // call function
    if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
        std::string errorFmt = "Lua OnUpdate error: " + std::string{lua_tostring(L, -1)} + "\n";
        Core::Logger::log(Core::LogLevel::Error, errorFmt);
        lua_pop(L, 1);
    }
}

};
