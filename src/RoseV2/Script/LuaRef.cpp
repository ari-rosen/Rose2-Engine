#include "RoseV2/Script/LuaRef.hpp"
#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"

namespace RoseV2::Script {

LuaRef::LuaRef(int ref) : m_Ref(ref) {}

LuaRef::~LuaRef() {
    reset();
}

LuaRef::LuaRef(LuaRef &&other) noexcept
    : m_Ref(other.m_Ref)
{
    other.m_Ref = LUA_NOREF;
}

LuaRef& LuaRef::operator=(LuaRef &&other) noexcept {
    if (this == &other) return *this;
    reset();
    m_Ref = other.m_Ref;
    other.m_Ref = LUA_NOREF;
    return *this;
}

void LuaRef::reset() {
    lua_State *L = LuaEngine::getLuaState();

    if (m_Ref != LUA_NOREF && L) {
        luaL_unref(L, LUA_REGISTRYINDEX, m_Ref);
        m_Ref = LUA_NOREF;
    }
}

void LuaRef::push() const {
    lua_State *L = LuaEngine::getLuaState();

    if (valid()) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_Ref);
    }
    else {
        lua_pushnil(L);
    }
}


}
