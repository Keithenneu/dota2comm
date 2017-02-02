local interactiveConsole = require(GetScriptDirectory().."/interactiveConsole")

local svenConsole = interactiveConsole:new("svenConsole")

function Think()
    svenConsole:execute()
end