#include "RoseV2/Script/RuntimeEntityTables.hpp"

#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"

namespace RoseV2::Script {

void RuntimeEntityTables::ensureEntityTable(Core::UUID uuid, const std::string& scriptName) {
    auto& scriptMap = m_EntityTables[uuid];
    if (scriptMap.find(scriptName) != scriptMap.end()) return;

    lua_State* L = LuaEngine::getLuaState();
    lua_newtable(L);
    LuaRef ref(luaL_ref(L, LUA_REGISTRYINDEX));

    auto& fields = LuaEngine::getFieldRegistry().getEntitySerializeableFields(uuid, scriptName);
    ref.push();
    for (auto& field : fields) {
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

    scriptMap[scriptName] = std::move(ref);
}

LuaRef& RuntimeEntityTables::getEntityTable(Core::UUID uuid, const std::string& scriptName) {
    ensureEntityTable(uuid, scriptName);
    return m_EntityTables[uuid][scriptName];
}

void RuntimeEntityTables::cleanup(Scene::Scene& scene) {
    for (auto it = m_EntityTables.begin(); it != m_EntityTables.end();) {
        Core::UUID uuid = it->first;
        ECS::Entity entity = scene.getEntityFromUUID(uuid);

        if (entity == ECS::Entity::Null() || !scene.getRegistry().isEntityAlive(entity)) {
            it = m_EntityTables.erase(it);
        } else {
            ++it;
        }
    }
}

void RuntimeEntityTables::removeEntity(Core::UUID uuid) {
    m_EntityTables.erase(uuid);
}

};
