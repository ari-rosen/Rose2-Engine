#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Asset/AssetManager.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"
#include "RoseV2/Script/RuntimeEntityTables.hpp"
#include "RoseV2/Script/ScriptFieldRegistry.hpp"
#include "RoseV2/Script/ScriptGlue.hpp"
#include "RoseV2/Script/ScriptInstance.hpp"

namespace RoseV2::Script {

lua_State* LuaEngine::s_LuaState = nullptr;
Scene::Scene* LuaEngine::s_ActiveScene = nullptr;
ScriptFieldRegistry LuaEngine::s_FieldRegistry{};
RuntimeEntityTables LuaEngine::s_RuntimeEntityTables{};

void LuaEngine::init() {
    s_LuaState = luaL_newstate();
    luaL_openlibs(s_LuaState);
}

void LuaEngine::shutdown() {
    if (s_LuaState) {
        lua_close(s_LuaState);
        s_LuaState = nullptr;
    }
}

void LuaEngine::onRuntimeStart(Scene::Scene *scene) {
    s_ActiveScene = scene;
    Script::ScriptGlue::registerLuaFunctions();

    s_RuntimeEntityTables = RuntimeEntityTables{};
}

void LuaEngine::onRuntimeStop() {
    s_ActiveScene = nullptr;
}

void LuaEngine::onStartEntity(ECS::Entity e) {
    auto sc = s_ActiveScene->getRegistry().getComponent<ECS::ScriptComponent>(e); 
    
    auto instance = Asset::AssetManager::getAsset<ScriptInstance>(sc.ScriptHandle);
    if (instance)
        instance->invokeOnStart(e);
}

void LuaEngine::onUpdateEntity(ECS::Entity e, Core::Timestep ts) {
    s_RuntimeEntityTables.cleanup(*s_ActiveScene);
    auto sc = s_ActiveScene->getRegistry().getComponent<ECS::ScriptComponent>(e);

    auto instance = Asset::AssetManager::getAsset<ScriptInstance>(sc.ScriptHandle);
    
    if (instance)
        instance->invokeOnUpdate(e, ts);
}

int LuaEngine::bytecodeWriter(lua_State *L, const void *p, size_t size, void *data) {
    std::vector<std::byte> *bytecode_buffer = static_cast<std::vector<std::byte>*>(data);
    
    const std::byte* bytes = reinterpret_cast<const std::byte*>(p);
    bytecode_buffer->insert(bytecode_buffer->end(), bytes, bytes + size);
    
    return 0;
}

}

extern "C" int luaBytecodeWriterWrapper(lua_State *L, const void *p, size_t size, void *data) {
    return RoseV2::Script::LuaEngine::bytecodeWriter(L, p, size, data);
}
