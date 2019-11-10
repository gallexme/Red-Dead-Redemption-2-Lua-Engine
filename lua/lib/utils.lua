function printTable(t, f)

    local function printTableHelper(obj, cnt)
 
       local cnt = cnt or 0
 
       if type(obj) == "table" then
 
          io.write("\n", string.rep("\t", cnt), "{\n")
          cnt = cnt + 1
 
          for k,v in pairs(obj) do
 
             if type(k) == "string" then
                io.write(string.rep("\t",cnt), '["'..k..'"]', ' = ')
             end
 
             if type(k) == "number" then
                io.write(string.rep("\t",cnt), "["..k.."]", " = ")
             end
 
             printTableHelper(v, cnt)
             io.write(",\n")
          end
 
          cnt = cnt-1
          io.write(string.rep("\t", cnt), "}")
 
       elseif type(obj) == "string" then
          io.write(string.format("%q", obj))
 
       else
          io.write(tostring(obj))
       end 
    end
 
    if f == nil then
       printTableHelper(t)
    else
       io.output(f)
       io.write("return")
       printTableHelper(t)
       io.output(io.stdout)
    end
end
function inspect(o,indent)
    if indent == nil then indent = 0 end
    local indent_str = string.rep("    ", indent)
    local output_it = function(str)
        print(indent_str..str)
    end

    local length = 0

    local fu = function(k, v)
        length = length + 1
        if type(v) == "userdata" or type(v) == 'table' then
            output_it(indent_str.."["..k.."]")
            inspect(v, indent+1)
        else
            output_it(indent_str.."["..k.."] "..tostring(v))
        end
    end

    local loop_pairs = function()
        for k,v in pairs(o) do fu(k,v) end
    end

    local loop_metatable_pairs = function()
        for k,v in pairs(getmetatable(o)) do fu(k,v) end
    end

    if not pcall(loop_pairs) and not pcall(loop_metatable_pairs) then
        output_it(indent_str.."[[??]]")
    else
        if length == 0 then
            output_it(indent_str.."{}")
        end
    end
end