LOADEDMODS = {}
LOADEDLIBS = {}
function safe_wrap(func)
    return (function(...)
        local status, result = pcall(func, ...)
        if not status then -- resultor handle
            print(result, status)
        else
            return result
        end
    end)
end
function get_mods()
    if (#LOADEDMODS == 0) then
        for mod in lfs.dir("scripts/lua/mod") do
            if mod ~= "." and mod ~= ".." and mod:find("%.lua") ~= nil then
                local modName = string.gsub(mod, ".lua", "")
                local mod = require(modName)
                if (mod ~= false and type(mod) == "table") then
                    for key, maybefunc in pairs(mod) do
                        if type(maybefunc) == "function" then
                            mod[key] = safe_wrap(maybefunc)
                        end
                    end

                    table.insert(LOADEDMODS, mod)
                end
            end
        end
    end
    return LOADEDMODS
end
function get_libs()
    if (#LOADEDLIBS == 0) then
        for lib in lfs.dir("scripts/lua/lib") do
            if lib ~= "." and lib ~= ".." and lib:find("%.lua") ~= nil then
                local libName = string.gsub(lib, ".lua", "")
                local lib = require(libName)
                if (lib ~= false and type(lib) == "table") then
                    for key, maybefunc in pairs(lib) do
                        if type(maybefunc) == "function" then
                            lib[key] = safe_wrap(maybefunc)
                        end
                    end
                    table.insert(LOADEDLIBS, lib)
                end
            end
        end
    end
    return LOADEDLIBS
end
get_mods = safe_wrap(get_mods)
get_libs = safe_wrap(get_libs)

local function init()

    package.path = "scripts/lua/lib/" .. "?.lua;" .. "scripts/lua/mod/" ..
                       "?.lua;" .. "scripts/lua/" .. "?.lua;" .. package.path
    local libs = get_libs()
    for libname, lib in pairs(libs) do
        if type(lib.init) == "function" then lib.init() end
    end

    local mods = get_mods()
    for modname, mod in pairs(mods) do mod.init() end
    local coroutines = {}
    for modname, mod in pairs(mods) do
        table.insert(coroutines, coroutine.create(  mod.main))
    end
    -- Let a Mod do its Job
    while (#coroutines ~= 0) do
      if (LUAENGINE.IsKeyDown(Keys.F1)) then
        print("RESTART")
        LUAENGINE.ScriptWait(2500)
        return
      end
        local co = table.remove(coroutines)
        if coroutine.status(co)  ~= "dead"  then
            coroutine.resume(co)
            if coroutine.status(co)  ~= "dead" then -- Still got Work Todo, put back in List
                table.insert(coroutines, co)
            end
        end

        LUAENGINE.ScriptWait(0)
    end

end
function on_key_up(key)
    local mods = get_mods()
    for modname, mod in pairs(mods) do mod.on_key_up(key) end
end
function on_key_down(key)
    local mods = get_mods()
    for modname, mod in pairs(mods) do mod.on_key_down(key) end
end

on_key_up = safe_wrap(on_key_up)
on_key_down = safe_wrap(on_key_down)
init = safe_wrap(init)

tick = safe_wrap(tick)
init()
