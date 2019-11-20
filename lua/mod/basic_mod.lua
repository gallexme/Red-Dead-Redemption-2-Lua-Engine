local mod = {}
local modelstoload = {}

local function AddLoadModel(model_name, callback)
    if IS_MODEL_VALID(model_name) then
        table.insert(modelstoload,
                     {model_name = model_name, callback = callback})
    end
    -- print(IS_MODEL_VALID(model_name),HAS_MODEL_LOADED(model_name),model_name)
end

local function LoadModel()
    local tick = 0
    while (#modelstoload ~= 0) do
        
        tick = tick +1
        print("LoadModel RUNNING SMTH: ".. tick)
        local job = table.remove(modelstoload)
        if HAS_MODEL_LOADED(job.model_name) == 0 then
            REQUEST_MODEL(job.model_name, 0)
            print(job.model_name, 0)
            if HAS_MODEL_LOADED(job.model_name) == 0 then
                table.insert(modelstoload, job)
            else

                job.callback()
            end
        else

            coroutine.yield()
            job.callback()
        end

        LUAENGINE.ScriptWait(0)
        coroutine.yield()
    end

    LUAENGINE.ScriptWait(0)
    coroutine.yield()
end
local function spawnped(model_name)
    print(model_name)
    model_name = GET_HASH_KEY(model_name)
    local player_ped = GET_PLAYER_PED(0)
    local x, y, z = GET_ENTITY_COORDS(player_ped, false)
    AddLoadModel(model_name, function()
        -- print(x,y,z)
        -- print(model_name)
        print("Finally Loaded Model", model_name)
        local createdPed = CREATE_PED(model_name, x, y, z, 0, false, false, 0,
                                      false)
        if createdPed ~= 0 then
            if IS_PED_IN_GROUP(player_ped) then
                print("YES")
                print(GET_PLAYER_GROUP(player_ped))
            else
                return
            end
            local player_group = GET_PED_RELATIONSHIP_GROUP_HASH(player_ped)

            print(createdPed, "NEW PED")
            SET_PED_AS_GROUP_MEMBER(createdPed, player_group)
            SET_ENTITY_CAN_BE_DAMAGED_BY_RELATIONSHIP_GROUP(createdPed, false,
                                                            player_group)
            -- print(createdPed,GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(0,true))
            print(GET_PED_RELATIONSHIP_GROUP_DEFAULT_HASH(player_ped))
            print(GET_PED_RELATIONSHIP_GROUP_DEFAULT_HASH(createdPed))
            -- SET_GROUP_SEPARATION_RANGE ( int groupHandle, float separationRange ) 

            SET_ENTITY_VISIBLE(createdPed, 1)
            --SET_ENTITY_ALPHA(createdPed, 255, 0)
            SET_PED_AS_COP(createdPed)
            SET_PED_MAX_HEALTH(player_ped,10000)
            SET_PED_RANDOM_COMPONENT_VARIATION(createdPed, math.random(5))
            SET_GROUP_FORMATION(player_group, 1)

            SET_GROUP_SEPARATION_RANGE(player_group, 1)
        end
        SET_MODEL_AS_NO_LONGER_NEEDED(model_name)

        -- SET_ENTITY_COORDS_NO_OFFSET(player_ped,2700.1, -1403.39, 46.637,0,0,0,0)
    end)

end
local function renderBox(x, y, width, height, color1, color2, color3, color4)
    local varString = _CREATE_VAR_STRING(10, "LITERAL_STRING",
                                         "TESTTTTTTTTTTTTTTTTTT")

    _DRAW_TEXT(varString, 1, 1)

    -- DRAW_RECT(x, y, width, height, color1, color2, color3, color4, true)
end

function mod.main()
    while (true) do
        LoadModel()
        if (LUAENGINE.IsKeyDown(Keys.F5)) then
            --pawnped("U_M_Y_CzPHomesteadSon_02")

            coroutine.yield()
            LUAENGINE.ScriptWait(1000)

        elseif (LUAENGINE.IsKeyDown(Keys.F1)) then
            
            -- Restart Lua
            return
        elseif (LUAENGINE.IsKeyDown(Keys.F2)) then
            local player_ped = GET_PLAYER_PED(0)
            
            SET_PED_AS_COP(player_ped)
            SET_PED_MAX_HEALTH(player_ped,10000)
            LUAENGINE.ScriptWait(1000)
        end
       
    LUAENGINE.ScriptWait(100)   
    -- print("test")
        coroutine.yield()
        -- print("HEllo from new LuaEngine")

    end
end
function mod.init() end

-- print(IS_MODEL_VALID(model_name),HAS_MODEL_LOADED(model_name),model_name)

return mod
