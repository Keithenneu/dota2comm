local interactiveConsole = require(GetScriptDirectory().."/interactiveConsole")

local linaConsole = interactiveConsole:new("linaConsole")

function Think()
    linaConsole:execute()
end