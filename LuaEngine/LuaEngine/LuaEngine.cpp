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

            nativeInit(native);
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
                if (is_in(argument,
                    "BOOL",
                    "Entity",
                    "Player",
                    "FireId",
                    "Ped",
                    "Vehicle",
                    "Cam",
                    "CarGenerator",
                    "Group",
                    "Train",
                    "Pickup",
                    "Object",
                    "Weapon",
                    "Interior",
                    "Blip",
                    "Texture",
                    "TextureDict",
                    "CoverPoint",
                    "Camera",
                    "TaskSequence",
                    "ColourIndex",
                    "Sphere",
                    "ScrHandle",
                    "int"
                )) {
                    nativePush((Player)lua_tointeger(L, argIdx++));
                    /*"Player", "int", "Ped", "Void", "Entity", "Vehicle", "Object", "Cam", "BOOL", "Blip", "Pickup", "Vehicle*", "FireId", "ScrHandle", "Entity*", "Blip*", "BOOL*", "Hash*", "float*"*/
                }
                else  if (is_in(argument, "Vehicle*", "Entity*", "Blip*", "BOOL*", "Hash*", "float*", "Any*")) {
                    nativePush((Any)lua_tointeger(L, argIdx++));
                    Log::Error << "[TODO]Pointer Type: " << argument;
                }
                else  if (is_in(argument, "Void", "Any", "uint", "Hash")) {
                    nativePush((Any)lua_tointeger(L, argIdx++));
                }
                else if (is_in(argument, "float")) {
                    nativePush((float)lua_tonumber(L, argIdx++));
                }
                else if (is_in(argument, "bool")) {
                    nativePush((bool)lua_tonumber(L, argIdx++));
                }
                else if (argument == "Vector3") {
                    Vector3 vecToPush = Vector3();
                    vecToPush.x = (float)lua_tonumber(L, argIdx++);
                    vecToPush.y = (float)lua_tonumber(L, argIdx++);
                    vecToPush.z = (float)lua_tonumber(L, argIdx++);
                }
                else if (is_in(argument, "const char*", "char*")) {
                    nativePush(lua_tostring(L, argIdx++));
                }
                else {
                    Log::Error << "Unknown Arg: " << argument;
                    argIdx++;
                    return 0;
                }
            }
            int returnCount = 0;
            if (is_in(return_type, "BOOL",
                "Entity",
                "Player",
                "FireId",
                "Ped",
                "Vehicle",
                "Cam",
                "CarGenerator",
                "Group",
                "Train",
                "Pickup",
                "Object",
                "Weapon",
                "Interior",
                "Blip",
                "Texture",
                "TextureDict",
                "CoverPoint",
                "Camera",
                "TaskSequence",
                "ColourIndex",
                "Sphere",
                "ScrHandle",
                "int")) {
                Player res = nativeCall<Player>();
                lua_pushinteger(L, res);
                returnCount = 1;
            }
            else if (return_type == "float") {
                float res = nativeCall<float>();
                lua_pushnumber(L, res);
                returnCount = 1;
            }
            else if (return_type == "Vector3") {
                Vector3 res = nativeCall<Vector3>();
                lua_pushnumber(L, res.x);
                lua_pushnumber(L, res.y);
                lua_pushnumber(L, res.z);
                returnCount = 3;
            }
            else if (is_in(return_type, "const char*", "char*")) {
                const char* res = nativeCall<const char*>();
                if (res != nullptr && res[0] != '\0') {
                    lua_pushstring(L, res);
                }
                else {
                    lua_pushboolean(L, false);
                }

                //lua_pushinteger(L, ctx.ResultPointer());
                returnCount = 1;
            }
            else  if (is_in(return_type, "void", "Void", "Any", "uint", "Hash")) {
                Void res = nativeCall<Void>();
                lua_pushinteger(L, res);
                returnCount = 1;
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

    int Methods::IsKeyDown(lua_State* L) {
        luaL_checktype(L, 1, LUA_TNUMBER);
        DWORD key = lua_tointeger(L, 1);
        lua_pushboolean(L, ::IsKeyDown(key));

        return 1;
    }
    int Methods::IsKeyJustUp(lua_State* L) {
        luaL_checktype(L, 1, LUA_TNUMBER);
        bool exclusive = true;
        if (lua_isboolean(L, 2)) {
            exclusive = lua_toboolean(L, 2);
        }
        DWORD key = lua_tointeger(L, 1);
        lua_pushboolean(L, ::IsKeyJustUp(key, exclusive));

        return 1;
    }
    int Methods::IsKeyDownLong(lua_State* L) {
        luaL_checktype(L, 1, LUA_TNUMBER);
        DWORD key = lua_tointeger(L, 1);
        lua_pushboolean(L, ::IsKeyDownLong(key));

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
    int Methods::WorldGetAllVehicles(lua_State* L) {
        const int ARR_SIZE = 1024;
        Vehicle vehicles[ARR_SIZE];
        int count = worldGetAllVehicles(vehicles, ARR_SIZE);
        lua_createtable(L, ARR_SIZE, 1);
        int newTable = lua_gettop(L);

        for (int i = 0; i < ARR_SIZE; i++) {
            lua_pushinteger(L, vehicles[i]);
            lua_rawseti(L, newTable, i + 1);
        }
        return 1;
    }
    int Methods::WorldGetAllPeds(lua_State* L) {
        const int ARR_SIZE = 1024;
        Ped peds[ARR_SIZE];
        int count = worldGetAllPeds(peds, ARR_SIZE);
        lua_createtable(L, ARR_SIZE, 1);
        int newTable = lua_gettop(L);

        for (int i = 0; i < ARR_SIZE; i++) {
            lua_pushinteger(L, peds[i]);
            lua_rawseti(L, newTable, i + 1);
        }
        return 1;
    }
    int Methods::WorldGetAllObjects(lua_State* L) {
        const int ARR_SIZE = 1024;
        Object objects[ARR_SIZE];
        int count = worldGetAllObjects(objects, ARR_SIZE);
        lua_createtable(L, ARR_SIZE, 1);
        int newTable = lua_gettop(L);

        for (int i = 0; i < ARR_SIZE; i++) {
            lua_pushinteger(L, objects[i]);
            lua_rawseti(L, newTable, i + 1);
        }
        return 1;
    }
    int Methods::WorldGetAllPickups(lua_State* L) {
        const int ARR_SIZE = 1024;
        Pickup pickups[ARR_SIZE];
        int count = worldGetAllPickups(pickups, ARR_SIZE);
        lua_createtable(L, ARR_SIZE, 1);
        int newTable = lua_gettop(L);

        for (int i = 0; i < ARR_SIZE; i++) {
            lua_pushinteger(L, pickups[i]);
            lua_rawseti(L, newTable, i + 1);
        }
        return 1;
    }
    static const luaL_reg modfuncs[] =
    {
        { "CallNative", Methods::CallNative},
        { "ScriptWait", Methods::Wait},
        { "IsKeyDown", Methods::IsKeyDown},
        { "IsKeyDownLong", Methods::IsKeyDownLong},
        { "IsKeyJustUp", Methods::IsKeyJustUp},
        { "ResetKeyState", Methods::ResetKeyState},
        { "WorldGetAllVehicles", Methods::WorldGetAllVehicles},
        { "WorldGetAllPeds", Methods::WorldGetAllPeds},
        { "WorldGetAllObjects", Methods::WorldGetAllObjects},
        { "WorldGetAllPickups", Methods::WorldGetAllPickups},
        { NULL, NULL }
    };
    void Engine::ScriptRegister()
    {
        luaEngine = new Engine();
        luaEngine->Init();
        luaEngine->Run();
    }
    void Engine::BeforeScriptUnregister()
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

        luaL_register(LuaState, "LUAENGINE", modfuncs);

        std::ostringstream LuaMainPath;
        LuaMainPath << LuaEnginePath.c_str() << "/scripts/lua/main.lua";
        Log::Info << "Lua Restarted, Starting Main" << Log::Endl;
        auto error = luaL_dofile(LuaState, LuaMainPath.str().c_str());
        if (error) {
            // the top of the stack should be the error string
            auto err = lua_tostring(LuaState, -1); // read the LuaJIT error, works too
            Log::Error << "Lua Error: " << err << Log::Endl;
            lua_pop(LuaState, 1);

            lua_close(LuaState);
            LuaState = nullptr;
            return;
        }
        /*std::cout << "IS_MODEL_VALID: " << IS_MODEL_VALID();
        std::cout << "HAS_MODEL_LOADED: " << HAS_MODEL_LOADED();
        std::cout << "REQUEST_MODEL ";
        std::cout << "HAS_MODEL_LOADED: " << HAS_MODEL_LOADED();
        */
        Run();
    }

 

    Engine::~Engine()
    {
        lua_close(LuaState);
    }
}