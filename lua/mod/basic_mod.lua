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
function drawtext(x,  y, text,  r, g,  b,  a,  scaleX ,  scaleY )

	local fX = x / 1920;
  local fY = y / 1080;
  native("HUD","SET_TEXT_SCALE",scaleX,scaleY)
	native("HUD","SET_TEXT_COLOR", r, g, b, a);
	local varString,x,y = native("MISC","_CREATE_VAR_STRING", 10, "LITERAL_STRING", text)
  print("value Start",varString,"value end")
  native("HUD","DRAW_TEXT", x, fX, fY)
end
function mod.tick()
  renderBox(1,1,1000,1000,255,255,255,70)
  --drawtext(100,100,"Test String, Hello World from Lua",255,0,255,255,0.2,0.2)

  end

function mod.on_key_up(key)
  if key == Keys.Tab then

    local ped,x = native("PLAYER", "GET_PLAYER_PED", 0)
    local x, y, z = native("ENTITY", "GET_ENTITY_COORDS", ped, false)
    print("Player Ped Id is", ped)
    print("Player Ped Id is At Location", x, y, z)

  end
end
 function mod.on_key_down(key)

end


return mod