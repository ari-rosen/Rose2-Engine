#include <gtest/gtest.h>

#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Script/ScriptInstance.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"

using namespace RoseV2::Script;
using namespace RoseV2;

class LuaTests : public ::testing::Test {
protected:
    void SetUp() override {
        LuaEngine::shutdown();
        LuaEngine::init();
    }

    void TearDown() override {
        LuaEngine::shutdown();
    }
};

TEST_F(LuaTests, IsInitialized) {
    ASSERT_NE(LuaEngine::getLuaState(), nullptr);
}

TEST_F(LuaTests, OpenLibsSucceeded) {
    lua_State* L = LuaEngine::getLuaState();
    
    lua_getglobal(L, "print");
    
    ASSERT_TRUE(lua_isfunction(L, -1));
    
    lua_pop(L, 1);
}

TEST_F(LuaTests, LoadSource_ValidScript_Succeeds) {
    const std::string scriptName = "TestScript";
    const std::string source = 
        "TestScript = {}\n"
        "function TestScript:onStart()\n"
        "end\n";

    ScriptInstance script(scriptName);
    ASSERT_TRUE(script.loadSource(source));
}

TEST_F(LuaTests, LoadSource_MissingTable_Fails) {
    ECS::Entity entity{1, 0};
    const std::string scriptName = "WrongName";
    const std::string source = 
        "CorrectName = {}\n"
        "function CorrectName:onStart()\n"
        "end\n";

    ScriptInstance script(scriptName);
    ASSERT_FALSE(script.loadSource(source));

    lua_State* L = LuaEngine::getLuaState();
    lua_getglobal(L, scriptName.c_str());
    ASSERT_TRUE(lua_isnil(L, -1));
    lua_pop(L, 1);
}

int g_LuaTestValue = 0;

TEST_F(LuaTests, InvokeOnStart_FunctionIsCalled) {
    auto lua_set_test_value = [](lua_State* L) -> int {
        g_LuaTestValue = 42;
        return 0;
    };
    
    lua_State* L = LuaEngine::getLuaState();
    lua_register(L, "SetTestValue", lua_set_test_value);
    
    const std::string scriptName = "Starter";
    const std::string source = 
        "Starter = {}\n"
        "function Starter:onStart()\n"
        "   SetTestValue()\n"
        "end\n";

    ScriptInstance script(scriptName);
    ASSERT_TRUE(script.loadSource(source));
    
    script.invokeOnStart({});
    
    ASSERT_EQ(g_LuaTestValue, 42) << "The Lua onStart function was not executed.";
}

int g_LuaArgumentReceived = 0;

TEST_F(LuaTests, InvokeOnUpdate_BasicFunctionality) {
    auto lua_set_test_value = [](lua_State* L) -> int {
        g_LuaTestValue = 99;
        return 0;
    };
    
    auto lua_grab_arg = [](lua_State* L) -> int {
        double ts_ms = lua_tonumber(L, 1);
        g_LuaArgumentReceived = static_cast<int>(ts_ms);
        return 0;
    };
    
    lua_State* L = LuaEngine::getLuaState();
    lua_register(L, "SetCheckValue", lua_set_test_value);
    lua_register(L, "GrabArg", lua_grab_arg);

    ECS::Entity entity{3, 0};
    
    const std::string scriptName = "SimpleUpdate";
    const std::string source = 
        "SimpleUpdate = {}\n"
        "function SimpleUpdate:onUpdate(deltaTime)\n"
        "   SetCheckValue()\n"
        "   GrabArg(deltaTime)\n"
        "end\n";

    ScriptInstance script(scriptName);
    
    ASSERT_TRUE(script.loadSource(source)) << "Test failed to load the valid script source.";
    
    Core::Timestep testTS(0.016f);
    script.invokeOnUpdate({}, testTS);
    
    ASSERT_EQ(g_LuaTestValue, 99) << "The Lua onUpdate function was not executed.";
    
    ASSERT_EQ(g_LuaArgumentReceived, 16) << "Timestep argument was not correctly passed to the Lua function.";
}
