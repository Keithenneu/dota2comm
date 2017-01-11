require(GetScriptDirectory().."/dota2comm")


function DoSomething()
	local msg = comm.receive(); -- try to receive a message
	if msg ~= nil then -- if there was one
		print(msg); -- print it
	end
    
    
    comm.send("foobar"); -- send a message
end