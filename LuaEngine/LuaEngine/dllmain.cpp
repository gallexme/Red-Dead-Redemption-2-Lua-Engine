// dllmain.cpp : Определяет точку входа для приложения DLL.

#include <functional>
#include <deque>
#include <string>
#include <chrono>

#include "common.h"
#include "lfs/lfs.h"
#include "main.h"
#include "types.h"
#include <sstream>
#include "LuaEngine.h"
#include "keyboard.hpp"
ALT_LOG_IMPL;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        Lua::HModule = hModule;
        scriptRegister(hModule, Lua::Engine::ScriptRegister);
        keyboardHandlerRegister(OnKeyboardMessage);
        break;

    case DLL_PROCESS_DETACH:
        scriptUnregister(hModule);
        Log::Error << "Closing Stuff Detach Happend";
        Lua::Engine::BeforeScriptUnregister();
        keyboardHandlerUnregister(OnKeyboardMessage);
        break;
    }
    return TRUE;
}