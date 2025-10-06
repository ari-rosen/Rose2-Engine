#pragma once

#include "RoseV2/Script/LuaHeaders.hpp"

namespace RoseV2::Script {

class LuaRef {
public:
    LuaRef(int ref = LUA_NOREF);
    ~LuaRef();

    LuaRef(const LuaRef&) = delete;
    LuaRef& operator=(const LuaRef&) = delete;
    
    LuaRef(LuaRef &&other) noexcept;
    LuaRef& operator=(LuaRef &&other) noexcept;

    void reset();
    void push() const;

    inline int get() const { return m_Ref; };
    inline bool valid() const { return m_Ref != LUA_NOREF; };
private:
    int m_Ref;
};

};
