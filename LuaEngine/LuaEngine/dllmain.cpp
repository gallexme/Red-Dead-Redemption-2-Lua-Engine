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
            
            scriptRegister(hModule,Lua::Engine::ScriptRegister);
             Lua::HModule = hModule;
                //@TODO:        keyboardHandlerRegister(OnKeyboardMessage);
            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        {
            Lua::Engine::ScriptUnregister();

         //@TODO:    keyboardHandlerUnregister(OnKeyboardMessage);
            break;
        }
        return TRUE;
        }
    }

