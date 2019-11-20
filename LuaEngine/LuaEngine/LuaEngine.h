#pragma once
#include <functional>
#include <deque>
#include <string>
#include <chrono>

#include "common.h"
#include "lfs/lfs.h"
#include "main.h"
#include "nativeCaller.h"
#include "types.h"
#include <sstream>

namespace Lua {
    static HMODULE HModule;

    class Engine
    {
        lua_State* LuaState;

        std::string LuaEnginePath;
    public:
        static void ScriptRegister();
        static void BeforeScriptUnregister();

        void Init();
        void Shutdown();
        void Run();
        ~Engine();
    };

    static Engine* luaEngine;
    class Methods {
    public:
        static int CallNative(lua_State* L);
        static int IsKeyDown(lua_State* L);
        static int IsKeyJustUp(lua_State* L);
        static int IsKeyDownLong(lua_State* L);
        static int ResetKeyState(lua_State* L);
        static int Wait(lua_State* L);
        static int WorldGetAllVehicles(lua_State* L);
        static int WorldGetAllPeds(lua_State* L);
        static int WorldGetAllObjects(lua_State* L);
        static int WorldGetAllPickups(lua_State* L);
    };
}