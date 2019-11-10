--print(call_native(0x275F255ED201B937,0))
--return call_native(0x275F255ED201B937,1)
local NativeDB = {}


namespace_hash_map = {}


function native(namespace,name,...)
    local func = namespace_hash_map[namespace][name]
    return natives.call_native(func,...)
end
local  function load_natives(natives) 
    local Json = require("json")
    local jsondb = Json.decode(natives:read("*a"))
    for namespace,native_list in pairs(jsondb) do
        local function_hashmap = {}
        for native, definition in pairs(native_list) do
            local func_def = {}

            func_def["native"] = native
            func_def["return_type"]=definition["return_type"]
            local args = {}
            local func = definition["return_type"].. " "..definition["name"].."("
            for k,param in pairs(definition["params"]) do
                func = func.. param["type"].." "..param["name"]
                if k ~=  #definition["params"]  then
                func = func..","
                end
                table.insert(args,param["type"])
            end
            func = func..")"
           -- print(func)
            func_def["args"] = args
            function_hashmap[definition["name"]] = func_def

        end
        namespace_hash_map[namespace] = function_hashmap
    end
    natives:close()

    
end


local function init()
    local natives = io.open("scripts/lua/natives.json","r")
    local status, err = pcall(load_natives,natives)
    
    if not status and natives then
        print(err,status)
      --   io.close(natives)
    end
    
end

NativeDB.init = init
return NativeDB
--print(natives.call_native("0x275F255ED201B937",0))