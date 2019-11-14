#include "LuaEngine.h"
#include "keyboard.hpp"
namespace Lua {

    int  Methods::CallNative(lua_State* L)
    {
        try {
            int nargs = lua_gettop(L);
            luaL_checktype(L, 1, LUA_TTABLE);
            lua_getfield(L, 1, "return_type");
            std::string return_type = std::string(lua_tostring(L, -1));
            lua_getfield(L, 1, "native");
            const char* native_string = lua_tostring(L, -1);
            uint64_t native = strtoull(native_string, NULL, 0);

            invoker::NativeInit(native);
            lua_getfield(L, 1, "args");
            std::vector<std::string> argsTypes = std::vector<std::string>();

            lua_pushnil(L);
            while (lua_next(L, -2)) {
                const char* argString = lua_tostring(L, -1);
                argsTypes.push_back(argString);
                lua_pop(L, 1);
            }

            lua_pop(L, 1);
            int argIdx = 2;
            for (std::string argument : argsTypes) {

                if (is_in(argument, "int", "bool", "Ped", "Void", "Entity", "Vehicle", "Object", "Cam", "BOOL", "Any","Any*","char*","Blip","BOOL*","Pickup","Vehicle*","FireId","ScrHandle","Entity*","Blip*","BOOL*","Hash*","float*")) {
                    invoker::NativePush((Any)lua_tointeger(L, argIdx));

                }
                else if (argument == "float") {
                    invoker::NativePush((float)lua_tonumber(L, argIdx));
                }
                else if (argument == "Hash") {
                    invoker::NativePush((unsigned int)lua_tointeger(L, argIdx));
                }
                else if (argument == "Vector3") {

                    invoker::NativePush((float)lua_tonumber(L, argIdx));
                    invoker::NativePush((float)lua_tonumber(L, argIdx++));
                    invoker::NativePush((float)lua_tonumber(L, argIdx++));

                }
                else if (argument == "const char*") {
                    invoker::NativePush(lua_tostring(L, argIdx));
                }
                else {
                    std::cout << "Unknown Arg: " << argument;
                    return 0;
                }

                argIdx++;
            }
            int returnCount = 0;
            if (is_in(return_type, "int", "bool", "Ped", "Void", "Entity", "Vehicle","FireId","Interior","Pickup","Blip","ScrHandle", "Object", "Cam", "BOOL", "Any")) {
                Any res = invoker::NativeCall<Any>();
                lua_pushinteger(L, res);
                returnCount = 1;

            }
            else if (return_type == "float") {
                float res = invoker::NativeCall<float>();
                lua_pushnumber(L, res);
                returnCount = 1;
            }
            else if (return_type == "Hash") {
                Hash  res = invoker::NativeCall< Hash >();
                lua_pushinteger(L, res);
                returnCount = 1;
            }
            else if (return_type == "Vector3") {
                Vector3 res = invoker::NativeCall<Vector3>();
                lua_pushnumber(L, res.x);
                lua_pushnumber(L, res.y);
                lua_pushnumber(L, res.z);
                returnCount = 3;

            }
            else if (return_type == "const char*") {

                const char* res = invoker::NativeCall<const char*>();
                lua_pushstring(L, res);

                //lua_pushinteger(L, ctx.ResultPointer());
                returnCount = 1;
            }
            else if (return_type == "void") {

            }
            else {
                std::cout << "Unknown return_type: " << return_type;
                return 0;
            }

            return returnCount;
        }
        catch (...) {
            Log::Error << "NATIVE CALL FAILED UNKNOWN";
            return 0;
        }

        /*ctx.Push(player_id);
        Native::Invoke(Native::GetHandler(native, NULL, 0), ctx);

        lua_pushinteger(L, ctx.Result<uint32_t>());

        return 1;   */
    }

    int Methods::KeyDown(lua_State* L) {
        luaL_checktype(L, 1, LUA_TNUMBER);
        DWORD key = lua_tointeger(L, 1);
        if (::KeyDown(key)) {
            lua_pushboolean(L, true);
        }
        else {
            lua_pushboolean(L, false);
        }
        return 1;
    }
    int Methods::KeyJustUp(lua_State* L) {
        luaL_checktype(L, 1, LUA_TNUMBER);
        bool exclusive = true;
        if (lua_isboolean(L, 2)) {
            exclusive = lua_toboolean(L, 2);
        }
        DWORD key = lua_tointeger(L, 1);
        if (::KeyJustUp(key,exclusive)) {
            lua_pushboolean(L, true);
        }
        else {
            lua_pushboolean(L, false);
        }
        return 1;

    }
    int  Methods::KeyPressedOnce(lua_State* L){
        luaL_checktype(L, 1, LUA_TBOOLEAN);
        luaL_checktype(L, 2, LUA_TNUMBER);
        bool bIsPressed = lua_toboolean(L, 1);
        DWORD key = lua_tointeger(L, 2);
        if (::KeyPressedOnce(bIsPressed,key)) {
            lua_pushboolean(L, true);
        }
        else {
            lua_pushboolean(L, false);
        }
        return 1;

    }
    int  Methods::ResetKeyState(lua_State* L) {
        luaL_checktype(L, 1, LUA_TNUMBER);
        DWORD key = lua_tointeger(L, 1);
        ::ResetKeyState(key);
        
        return 0;

    }
    int Methods::Wait(lua_State* L)
    {
        luaL_checktype(L, 1, LUA_TNUMBER);
        DWORD time = lua_tointeger(L, 1);
        scriptWait(time);
        return 0;
    }

    static const luaL_reg modfuncs[] =
    {
        { "CallNative", Methods::CallNative},
        { "ScriptWait", Methods::Wait},
        { "KeyDown", Methods::KeyDown},
        { "KeyJustUp", Methods::KeyJustUp},
        { "KeyPressedOnce", Methods::KeyPressedOnce},
        { "ResetKeyState", Methods::ResetKeyState},
        { NULL, NULL }
    };
    void Engine::ScriptRegister()
    {
        luaEngine = new Engine();
        luaEngine->Init();
        luaEngine->Run();
    }
    void Engine::ScriptUnregister()
    {
        luaEngine->Shutdown();
        delete luaEngine;
    }
    void Engine::Init()
    {

        LuaEnginePath = GetModulePath(HModule);
        std::string logPath = LuaEnginePath + "/log.txt";

        Log::Push(new Log::FileStream(logPath));
        Log::Push(new Log::ConsoleStream());
        AllocConsole();
        SetConsoleTitle(L"LuaEngine");

        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);

        auto outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        const int width = 110;
        const int height = 30;

        // Add some more scrolling
        COORD size;
        size.X = width;
        size.Y = height * 10;
        SetConsoleScreenBufferSize(outputHandle, size);

        // Resize our console window
        SMALL_RECT rect;
        rect.Left = rect.Top = 0;
        rect.Right = width - 1;
        rect.Bottom = height - 1;
        SetConsoleWindowInfo(outputHandle, TRUE, &rect);
        Log::Info << "Natives registered" << Log::Endl;



    }

    void Engine::Shutdown()
    {

        /* cleanup Lua */
        if (LuaState != nullptr) {
            lua_close(LuaState);
            LuaState = nullptr;
        }
    }

    void Engine::Run()
    {
        Shutdown();

        LuaState = luaL_newstate();
        luaL_openlibs(LuaState);
        luaopen_lfs(LuaState);

        luaL_register(LuaState, "engine", modfuncs);

        std::ostringstream LuaMainPath;
        LuaMainPath << LuaEnginePath.c_str() << "/" << "/scripts/lua/main.lua";
        Log::Info << "Lua Restarted, Starting Main" << Log::Endl;
        auto error = luaL_dofile(LuaState, LuaMainPath.str().c_str());
        if (error) {
            // the top of the stack should be the error string
            auto err = lua_tostring(LuaState, -1); // read the LuaJIT error, works too
            Log::Error << "Lua Tick Error: " << err << Log::Endl;
            lua_pop(LuaState, 1);

            lua_close(LuaState);
            LuaState = nullptr;
            return;
        }
        Run();
    }



    void Engine::Stop()
    {
    }

    Engine::~Engine()
    {
        delete LuaState;
        LuaState = nullptr;
        delete this;
    }

}