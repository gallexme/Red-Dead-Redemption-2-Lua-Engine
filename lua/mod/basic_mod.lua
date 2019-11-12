local mod =  {}


function mod.init()
  

end
local function renderBox(x,y,width,height,color1,color2,color3,color4)
  local fWidth = width / 1920;
	local fHeight = height / 1080;
	local fX = (x + width / 2) / 1920;
	local fY = (y + height / 2) / 1080;
  native("GRAPHICS","DRAW_RECT",fX, fY,fWidth, fHeight, color1, color2, color3, color4,true);
end
local modelstoload = {}



function mod.tick()
  --renderBox(1,1,1000,1000,255,255,255,70)

  end


function mod.on_key_up(key)
  if key == Keys.Tab then

    local ped = native("PLAYER", "GET_PLAYER_PED", 0)
    local x, y, z = native("ENTITY", "GET_ENTITY_COORDS", ped, false)
    -- works now like
    ENTITY.GET_ENTITY_COORDS(ped,false)
    GET_ENTITY_COORDS(ped,false)
    

  end
end
 function mod.on_key_down(key)

end


return mod