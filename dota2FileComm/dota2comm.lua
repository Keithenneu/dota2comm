module("comm", package.seeall)

setfenv(1, comm);

currentrev = 0
queue = {}

function comm.send(text)
    print("dota2comm: "..text)
end

function comm.receive()
    result, err = loadfile(GetScriptDirectory().."/dota2comm/rev")
    if (err ~= nil) then
        return nil
    end
    result(); -- should set rev
    if (rev == nil) then rev = 0 end
    if (rev > currentrev) then
        currentrev = rev
        result, err = loadfile(GetScriptDirectory().."/dota2comm/messages")
        if (err ~= nil) then 
            print("commerror: "..err)
            return nil
        end
        result(); -- should set messages
        if (messages == nil) then messages = {} end
        for i = 1,#messages do
            if (messages[i] ~= "") then
                queue[#queue + 1] = messages[i]
            end
        end
    end
    if (#queue > 0) then
        return table.remove(queue, 1)
    end
end