local comm = {}

function comm:new(marker)
	print("comm init " .. marker)

    local o = {}
	setmetatable(o, self)
	self.__index = self

    o.marker = marker
    o.currentrev = 0
    o.queue = {}
    return o
end

function comm:send(text)
    print("dota2comm "..self.marker..": "..text)
end

function comm:receive()
    local result, err = loadfile(GetScriptDirectory().."/dota2comm/rev_"..self.marker)
    if (err ~= nil) then
        return nil
    end
    local env = {}
    setfenv(result, env)
    result() -- should set env.rev
    if (env.rev == nil) then env.rev = 0 end
    if (env.rev > self.currentrev) then
        self.currentrev = env.rev
        result, err = loadfile(GetScriptDirectory().."/dota2comm/messages_"..self.marker)
        if (err ~= nil) then
            print("commerror: "..err)
            return nil
        end
        local env = {}
        setfenv(result, env)
        result() -- should set env.messages
        if (env.messages == nil) then env.messages = {} end
        for i = 1,#env.messages do
            if (env.messages[i] ~= "") then
                self.queue[#self.queue + 1] = env.messages[i]
            end
        end
    end
    if (#self.queue > 0) then
        return table.remove(self.queue, 1)
    end
end

return comm
