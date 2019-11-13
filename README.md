# Red Dead Redemption 2 Lua Engine

LuaEngine is a ScriptHook Mod for writing LUA Mods/Scripts.

## Installation

Download the latest Release(https://github.com/gallexme/Red-Dead-Redemption-2-Lua-Engine/releases) and copy the LuaEngine.zip Contents into your GameFolder (we may gonna support outside GameFolder soon 

## Usage

F5 to Reload LuaEngine Ingame(should be changeable later on)
place mods in scripts/lua/mod folder
every Mod should return a Table with 
a init, tick, on_key_up, on_key_down Function

```lua
local mod =  {}
function mod.init() end
function mod.tick()  end

function mod.on_key_up(key)end
function mod.on_key_down(key)end
return mod
```

Natives Can be Called Like this

```lua
local ped = native("PLAYER", "GET_PLAYER_PED", 0)
local ped = PLAYER.GET_PLAYER_PED(0)
local ped = GET_PLAYER_PED(0)

```
Natives are defined in the natives.json(soon may come from a nativesdatabase)(please add ur changes back to the project)

Natives.json is Reloaded when LuaEngine Reloads ```F5```

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
