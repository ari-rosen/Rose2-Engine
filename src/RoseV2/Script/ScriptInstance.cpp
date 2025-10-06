#include "RoseV2/Script/ScriptInstance.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"
#include "RoseV2/Script/ScriptFieldRegistry.hpp"
#include "RoseV2/Script/ScriptGlue.hpp"

namespace RoseV2::Script {

ScriptInstance::ScriptInstance(const std::string &name)
    : m_Name(name) {}

bool ScriptInstance::loadSource(const std::string &source) {
    if (m_TableRef.valid()) {
        m_TableRef.reset(); 
    }

    lua_State *L = LuaEngine::getLuaState();

    if (luaL_loadstring(L, source.c_str()) != LUA_OK) {
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

    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return false;
    }

    lua_getfield(L, -1, functionName);

    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 2); 
        return false;
    }

    lua_insert(L, -2);

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

    if (!m_TableRef.valid()) return;

    m_TableRef.push();
    
    auto &allFields = ScriptFieldRegistry::getEntitySerializeableFields(e, m_Name);

    for (auto &field : allFields) {
        switch (field.Type) {
            case FieldType::Number:
                lua_pushnumber(L, std::get<double>(field.DefaultValue));
                break;
            case FieldType::Bool:
                lua_pushboolean(L, std::get<bool>(field.DefaultValue));
                break;
            case FieldType::String:
                lua_pushstring(L, std::get<std::string>(field.DefaultValue).c_str());
                break;
        }
        
        lua_setfield(L, -2, field.Name.c_str());
    }

    lua_pop(L, 1);

    if (!m_TableRef.valid()) return;
    if (!pushFunction("onStart")) return;
    if (!pushEntity(e)) return;

    if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
        std::string errorFmt = "Lua OnStart error: " + std::string{lua_tostring(L, -1)} + "\n";
        Core::Logger::log(Core::LogLevel::Error, errorFmt);
        lua_pop(L, 1);
    }
}

void ScriptInstance::invokeOnUpdate(ECS::Entity e, Core::Timestep ts) {
    lua_State *L = LuaEngine::getLuaState();

    if (!m_TableRef.valid()) return;
    if (!pushFunction("onUpdate")) return;
    if (!pushEntity(e)) return;

    lua_pushnumber(L, ts.getMiliseconds());

    if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
        std::string errorFmt = "Lua OnUpdate error: " + std::string{lua_tostring(L, -1)} + "\n";
        Core::Logger::log(Core::LogLevel::Error, errorFmt);
        lua_pop(L, 1);
    }
}

};
