local interactiveConsole = require(GetScriptDirectory().."/interactiveConsole")

local miranaConsole = interactiveConsole:new("miranaConsole")

function Think()
    miranaConsole:execute()
end