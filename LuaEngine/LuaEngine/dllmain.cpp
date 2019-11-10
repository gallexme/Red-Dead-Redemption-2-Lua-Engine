// dllmain.cpp : Определяет точку входа для приложения DLL.

#include "IMenu.h"
#include <functional>
#include <deque>
#include <string>
#include <chrono>

#include "common.h"
#include "lfs/lfs.h"


ALT_LOG_IMPL;

bool playerChanged = false;
bool playerTeleported = false;

uint32_t vehicles[] = { 47200842, 87577242, 89913743, 93893176, 150966765, 164947977, 217912393, 219205323, 253923078, 265500599, 374792535, 405368030, 470870473, 479008570, 518773733, 546463094, 572854005, 583628516, 661519966, 703203753, 742064790, 749266870, 822759307, 944661538, 962088794, 1016623270, 1115303244, 1192745176, 1297830025, 1308722738, 1468884243, 1493442814, 1582724764, 1588640480, 1596452133, 1642867136, 1745694728, 1761016051, 1770617692, 1786827347, 1871675108, 1874711736, 1975376854, 1998899080, 2109471093, 2111085321, 2135054537, 2186197941, 2241085408, 2272161463, 2306418508, 2348950726, 2428834674, 2433126343, 2466530028, 2468662606, 2536874959, 2541765679, 2584352890, 2586870135, 2606273646, 2681649752, 2743460295, 2750181085, 2818287139, 2868816046, 2907791640, 2919595783, 2947683355, 2986591923, 3005186165, 3015988546, 3194579596, 3262735550, 3268542718, 3286679556, 3289055845, 3295963005, 3306145232, 3338296319, 3371110816, 3435547054, 3470709364, 3628942640, 3658820774, 3671853927, 3724275886, 3750150746, 3791540180, 3889770730, 3897453428, 3901912305, 3916234491, 3917809588, 3962179608, 3973572878, 3999552582, 4001518838, 4011804713, 4019278719, 4036476012, 4059979704, 4114212256, 4114498448, 4130514530, 4151691040, 4158133943, 4159216269 };
constexpr uint32_t vehiclesCount = sizeof(vehicles) / 4;

static std::wstring GetModulePath(HMODULE module)
{
	DWORD size = MAX_PATH;
	std::vector<wchar_t> buffer(size);

	do
	{
		buffer.resize(size);
		GetModuleFileNameW(module, buffer.data(), size);
		size *= 1.5;
	} while (GetLastError() == ERROR_INSUFFICIENT_BUFFER);

	std::wstring modulePath = std::wstring(buffer.begin(), buffer.end());

	size_t slashPos = modulePath.size();
	for (int i = modulePath.size() - 1; i >= 0; --i)
	{
		if (modulePath[i] == L'/' || modulePath[i] == L'\\') {
			slashPos = i;
			break;
		}
	}

	std::wstring moduleDir = modulePath.substr(0, slashPos);
	return moduleDir;
}




void TickCpp()
{
}

static Native::Context ctx;
/*
[return_type] Any
[native] 0x275F255ED201B937
[args]
		[1] Any
*/
static int call_native(lua_State *L) {

	try {
		int nargs = lua_gettop(L);
		luaL_checktype(L, 1, LUA_TTABLE);
		lua_getfield(L, 1, "return_type");
		std::string return_type = std::string(lua_tostring(L, -1));
		lua_getfield(L, 1, "native");
		const char* native_string = lua_tostring(L, -1);
		uint64_t native = strtoull(native_string, NULL, 0);
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
			if (argIdx >= nargs) {
				break;
			}
			else
				if (argument == "Any") {
					ctx.Push((uint64_t)lua_tointeger(L, argIdx));

				}
				else if (argument == "int") {
					ctx.Push((uint64_t)lua_tointeger(L, argIdx));
				}
				else if (argument == "bool") {
					ctx.Push((bool)lua_tointeger(L, argIdx));
				}
				else if (argument == "float") {
					ctx.Push((float)lua_tonumber(L, argIdx));
				}
				else if (argument == "Hash") {
					ctx.Push(String::Hash(lua_tostring(L, argIdx)));
				}
				else if (argument == "Vector3") {

					ctx.Push((float)lua_tonumber(L, argIdx));
					ctx.Push((float)lua_tonumber(L, argIdx++));
					ctx.Push((float)lua_tonumber(L, argIdx++));

				}
				else if (argument == "const char*") {
					ctx.Push(lua_tostring(L, argIdx));
				}
				else {
					std::cout << "Unknown Arg: " << argument;
					return 0;
				}

			argIdx++;
		}
		Native::Invoke(Native::GetHandler(native), ctx);
		int returnCount = 0;
		if (return_type == "Any") {
			uint64_t res = ctx.Result<uint64_t>();
			lua_pushinteger(L, res);
			returnCount = 1;
		}
		else if (return_type == "int") {
			uint64_t res = ctx.Result<uint64_t>();
			lua_pushinteger(L, res);
			returnCount = 1;
		}
		else if (return_type == "bool") {
			bool res = ctx.Result<bool>();
			lua_pushboolean(L, res);
			returnCount = 1;
		}
		else if (return_type == "float") {
			float res = ctx.Result<float >();
			lua_pushnumber(L, res);
			returnCount = 1;
		}
		else if (return_type == "Hash") {
			uint64_t res = ctx.Result<uint64_t>();
			lua_pushinteger(L, res);
			returnCount = 1;
		}
		else if (return_type == "Vector3") {
			Vector3f res = ctx.Result<Vector3f>();
			lua_pushnumber(L, res.x);
			lua_pushnumber(L, res.y);
			lua_pushnumber(L, res.z);
			returnCount = 3;

		}
		else if (return_type == "const char*") {

			const char* res = ctx.Result<const char*>();
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
		ctx.Reset();

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
static const luaL_reg modfuncs[] =
{
	{ "call_native", call_native},

	{ NULL, NULL }
};

lua_State* L;
void LuaTick()
{

	try {
		if (L != nullptr) {
			/* the function name */
			lua_getglobal(L, "tick");
			if (lua_isfunction(L, -1)) {

				lua_call(L, 0, 0);
			}
			else {

			lua_pop(L, 1);
			}
		}
	}
	catch (...) {
		auto err = lua_tostring(L, -1); // read the LuaJIT error, works too
		Log::Error << "Lua Tick Error: " << err << Log::Endl;
	}

}
// Catch C++ exceptions and convert them to Lua error messages.
// Customize as needed for your own exception classes.
static int wrap_exceptions(lua_State *L, lua_CFunction f)
{

	Log::Info << "Wrapped Exception Called " << Log::Endl;
	try {

		Log::Info << "Safety First, no Call. "<<f << Log::Endl;

		lua_pushliteral(L, "caught (...)");
		return lua_error(L);
		return f(L);  // Call wrapped function and return result.
	}
	catch (const char *s) {  // Catch and convert exceptions.
		Log::Info << "Wrapped Exception: " <<s << Log::Endl;
		lua_pushstring(L, s);

	}

	catch (std::exception& e) {

		Log::Error << "Unknown Error: " << e.what()<<Log::Endl;
		lua_pushstring(L, e.what());
	}
	catch (...) {
		Log::Error << "Unknown Error: "<< Log::Endl;
		lua_pushliteral(L, "caught (...)");
	}

	Log::Info << "Return Wrapped Exception" << Log::Endl;
	return lua_error(L);  // Rethrow as a Lua error.
}

void RestartLua() {
	try {

		/* cleanup Lua */
		if (L != nullptr) {
			lua_close(L);
			L = nullptr;
		}

		L = luaL_newstate();
		luaL_openlibs(L);
		luaopen_lfs(L);
		/*lua_pushlightuserdata(L, (void *)wrap_exceptions);
		luaJIT_setmode(L, -1, LUAJIT_MODE_WRAPCFUNC | LUAJIT_MODE_ON);
		lua_pop(L, 1);
		//Dunno how this works
		*/
		luaL_register(L, "natives", modfuncs);

		Log::Info << "Lua Restarted, Starting Main" << Log::Endl;
		auto error = luaL_dofile(L, "scripts/lua/main.lua");
		if (error) {
			// the top of the stack should be the error string
			auto err = lua_tostring(L, -1); // read the LuaJIT error, works too
			Log::Error << "Lua Tick Error: " << err << Log::Endl;
			lua_pop(L, 1);

			lua_close(L);
			L = nullptr;
			return;
		}

		Log::Info << "Main Started " << Log::Endl;
	}
	catch (...) {
		auto err = lua_tostring(L, -1); // read the LuaJIT error, works too
		Log::Error << "Lua Error: " << err << Log::Endl;
	}

}
extern "C" {
	DLL_EXPORT void Init(Native::GetNativeAddressFunc getAddress)
	{
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
		Native::SetEssentialFunction(getAddress);
		Log::Info << "Natives registered" << Log::Endl;

		RestartLua();



		/* cleanup Lua */


	}

	DLL_EXPORT void Tick()
	{

		try {
			TickCpp();
			LuaTick();
		}
		catch (...) {
			Log::Error << "Unknown Error in Tick";
		}

	}

	DLL_EXPORT void OnKeyDown(uint32_t key)
	{

		try {
			if (L != nullptr) {

				lua_getglobal(L, "on_key_down");
				if (lua_isfunction(L, -1)) {
					lua_pushinteger(L, key);
					lua_call(L, 1, 0);

				}
				else {

					lua_pop(L, 1);
				}
				
			}
		}
		catch (...) {
			auto err = lua_tostring(L, -1); // read the LuaJIT error, works too
			Log::Error << "Lua OnKeyDown Error: " << err << Log::Endl;
		}

	}

	DLL_EXPORT void OnKeyUp(uint32_t key)
	{
		try {
			if (key == 0x74) {
				Log::Info << "User Requested Restart Lua" << Log::Endl;
				RestartLua();
				Log::Info << "User Lua Restarted" << Log::Endl;
			}

			if (L != nullptr) {

				lua_getglobal(L, "on_key_up");
				if (lua_isfunction(L, -1)) {
					lua_pushinteger(L, key);
					lua_call(L, 1, 0);

				}
				else {

					lua_pop(L, 1);
				}
				
			}
		}
		catch (...) {
			auto err = lua_tostring(L, -1); // read the LuaJIT error, works too
			Log::Error << "Lua OnKeyUp Error: " << err << Log::Endl;
		}
		/*if (key == 0x75) {
			auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < 100'000; ++i)
			{
				ctx.Push((uint32_t)0);
				Native::Invoke(Native::GetHandler(0x096275889B8E0EE0), ctx);

				ctx.Reset();
			}

			auto end = std::chrono::high_resolution_clock::now();

			auto duration = end - start;

			std::cout << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << std::endl;
		}*/

	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		std::wstring _moduleDir = GetModulePath(hModule);

		std::wstring logPath = _moduleDir + L"/log.txt";
		Log::Push(new Log::FileStream(logPath));
		Log::Push(new Log::ConsoleStream());
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

