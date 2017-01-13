# Dota2Comm

This is some code I wrote to allow for communication between the Dota2 Bot Lua Api/Sandbox, and just about every outside program you could imagine.

# Modules

-   dota2MemComm: A backend reading and writing into Dota's memory.
-   dota2FileComm: A backend for getting data into the Lua VM using lua files. No way out.
-   interactiveConsole: A usecase demonstration which allows the execution of any Lua code from a bot's context

# Backends

 | MemoryComm | FileComm
--- | --- | ---
*Speed* | quite fast | rather slow (3-4ms for reading)
*To Lua* | 1023byte per message (adjustable) | infinite
*From Lua* | infinite | not possible. Prints to Dota2 console.
*Platform* | currently implemented for windows | all
*Language* | all (which can handle dlls; python module included) | all (currently implemented in python)
*shady* | a little :D | definitely not.

## Contact
If you
-   find any bugs
-   want to know something
-   have a suggestion
-   Anything else

just let me know. Open an issue, write a pm, or whatever you want.
