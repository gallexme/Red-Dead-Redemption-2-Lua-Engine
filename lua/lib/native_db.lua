--print(CallNative(0x275F255ED201B937,0))
--return CallNative(0x275F255ED201B937,1)
local NativeDB = {}


namespace_hash_map = {}


function native(namespace,name,...)
    local func = namespace_hash_map[namespace][name]
    return LUAENGINE.CallNative(func,...)
end
local  function load_natives(nativedbFile) 
    local Json = require("json")
    local jsondb = Json.decode(nativedbFile:read("*a"))
    local argument_types = {}
    local return_types = {}
    for namespace,native_list in pairs(jsondb) do
        if(_G[namespace] == nil) then
            _G[namespace] = {}
        end
        local function_hashmap = {}
        for native, definition in pairs(native_list) do
            local func_def = {}

            func_def["native"] = native
            func_def["return_type"]=definition["return_type"]
            local args = {}
            local func = definition["return_type"].. " "..definition["name"].."("
            return_types[ definition["return_type"]] = 1
            for k,param in pairs(definition["params"]) do
                argument_types[ param["type"]] = 1
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
            local prototype =  function(...) return LUAENGINE.CallNative(func_def,...) end 
            _G[namespace][definition["name"]] = prototype
            _G[definition["name"]] =prototype
        end
        namespace_hash_map[namespace] = function_hashmap
    end
    
    local function gen_typings_database()
        for namespace,native_list in pairs(jsondb) do
            local native_file = io.open("scripts/lua/native_typings/"..namespace..".lua","w+")
            for native, definition in pairs(native_list) do
                local func =  "function "..definition["name"].."("
                local param_list = {}
                
                local typing_info = ""
                for k,param in pairs(definition["params"]) do
                    
                    param["name"] = "T"..param["name"]
                    
                    func = func.. param["name"]
                    
                    table.insert(param_list,param["name"])
                    if k ~=  #definition["params"]  then
                        func = func..","
                        
                    end
                    typing_info = typing_info.."---@param "..param["name"].." "..param["type"].."\n"
                end
            
                typing_info = typing_info.."---@return "..definition["return_type"].."\n"
                -- "return LUAENGINE.CallNative(".. table.concat(param_list,", ")..")
                func = typing_info..func..")  end"
                native_file:write(func.."\n")
            --print(func)
            end
            namespace_hash_map[namespace] = function_hashmap
            native_file:close()
        end
    end
    gen_typings_database()
   -- print("ARGUMENTS")
    --inspect(argument_types)
    
   -- print("RETURN TYPES")
    --inspect(return_types)
    nativedbFile:close()

    
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
--print(natives.CallNative("0x275F255ED201B937",0))