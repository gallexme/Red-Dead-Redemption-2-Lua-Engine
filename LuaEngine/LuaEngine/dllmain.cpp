// dllmain.cpp : Определяет точку входа для приложения DLL.

#include <functional>
#include <deque>
#include <string>
#include <chrono>

#include "common.h"
#include "lfs/lfs.h"
#include "redhook2.h"
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
        {

            Lua::HModule = hModule;
            keyboardHandlerRegister(OnKeyboardMessage);
            scriptRegister(hModule, Lua::Engine::ScriptRegister);
            //@TODO:        keyboardHandlerRegister(OnKeyboardMessage);
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            Lua::Engine::ScriptUnregister();
            scriptUnregister(hModule);
            keyboardHandlerUnregister(OnKeyboardMessage);
            //@TODO:    keyboardHandlerUnregister(OnKeyboardMessage);
            break;
        }
    }
    return TRUE;

}

