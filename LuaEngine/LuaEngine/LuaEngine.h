#pragma once
#include <functional>
#include <deque>
#include <string>
#include <chrono>

#include "common.h"
#include "lfs/lfs.h"
#include "redhook2.h"
#include "types.h"
#include <sstream>

namespace Lua {
    static HMODULE HModule;

    class Engine
    {
        lua_State* LuaState;

        std::wstring LuaEnginePath;
    public:
        static void ScriptRegister();
        static void ScriptUnregister();

        void Init();
        void Shutdown();
        void Run();
        void Stop();
        ~Engine();

    };

    static Engine* luaEngine;
    class Methods {
    public:
        static int CallNative(lua_State* L);


        static int Wait(lua_State* L);
        
    };

}