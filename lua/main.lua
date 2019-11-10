LOADEDMODS =  {}
LOADEDLIBS =  {}
function safe_wrap(func)
    return (function (...)
        local status, result = pcall(func, ...)
        if not status then--resultor handle
            print(result, status)
        else
            return result
        end
    end)
end
function get_mods()
    if (#LOADEDMODS == 0) then
      for mod in lfs.dir("scripts/lua/mod")do
        if mod ~= "."and mod ~= ".."and mod:find("%.lua")~= nil  then
          local modName = string.gsub(mod, ".lua", "")
          local mod = require(modName)
          if (mod ~= false and type(mod) == "table")then
            for key, maybefunc in pairs(mod)do
              if  type(maybefunc) == "function" then
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
      for lib in lfs.dir("scripts/lua/lib")do
        if lib ~= "."and lib ~= ".."and lib:find("%.lua")~= nil  then
          local libName = string.gsub(lib, ".lua", "")
          local lib = require(libName)
          if (lib ~= false and type(lib) == "table")then
            for key, maybefunc in pairs(lib)do
              if  type(maybefunc) == "function" then
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
function tick()
  -- print("TESTTTTTTTTTTT")--local status, err = pcall(script_tick)--if not status then--error handle--print(err, status)--end
    
    local mods = get_mods()
    for modname, mod in pairs(mods)do
       mod.tick()
    end
    
end
local function init()
    
	package.path = "scripts/lua/lib/" .. "?.lua;".. "scripts/lua/mod/" .. "?.lua;".."scripts/lua/" .. "?.lua;" .. package.path
    local libs = get_libs()
    for libname, lib in pairs(libs)do
       if type(lib.init) == "function" then 
       lib.init()
       end
    end

    local mods = get_mods()
    for modname, mod in pairs(mods)do
       mod.init()
    end

end
function on_key_up(key)
    local mods = get_mods()
    for modname, mod in pairs(mods)do
       mod.on_key_up(key)
    end
end
function on_key_down(key)
    local mods = get_mods()
    for modname, mod in pairs(mods)do
       mod.on_key_down(key)
    end
end

on_key_up = safe_wrap(on_key_up)
on_key_down = safe_wrap(on_key_down)
init = safe_wrap(init)

tick = safe_wrap(tick)
init()