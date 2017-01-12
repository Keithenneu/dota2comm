module("console", package.seeall)

setfenv(1, console);

require(GetScriptDirectory().."/dota2comm")

function makestring(o, f)
    if (not f) then f={} end
    
    if (type(o) == "number") then
        return ""..o
    end
    if (type(o) == "string") then
        return o
    end
    
    res = "{"
    f[o] = true
    for n in pairs(o) do
        res = res .. "\"" .. n .. "\""
    
        res = res .. ":"
    
        if (type(o[n]) == "number") then
            res = res .. o[n]
        elseif (type(o[n]) == "string") then
            res = res .. "\"" .. o[n] .. "\""
        else
            if (f[o[n]]) then
                res = res .. "\"recursion detected\""
            else
                res = res .. makestring(o[n])
            end
        end
    
        res = res .. ","
    end
    
    return string.sub(res, 0,string.len(res)-1) .. "}"
end

function execute()
	local msg = comm.receive(); -- get a message
    
    if msg == nil then -- if there wasn't one
		return; -- don't do anything
	end
    
    result = loadstring(msg)(); -- exceute it
    
    result = makestring(result); -- convert to string
    
    print(result)
    
    comm.send(result); -- send the result
end
