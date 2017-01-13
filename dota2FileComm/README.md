# Dota2MemComm

This backend utilises the Lua's loadfile function to read Lua files, prepared dynamically by the outside script.

There is currently no way of getting data from Lua to the outside.
(The console log files are flushed every 20 seconds or so, which is too slow for life output)

## Usage

### Lua part

(lua_example.lua)

````lua
    require(GetScriptDirectory().."/dota2comm")


    function DoSomething()
      local msg = comm.receive(); -- try to receive a message
      if msg ~= nil then -- if there was one
        print(msg); -- print it
      end


      comm.send("foobar"); -- send a message
    end
````

### Outside part

Use the included python module.
Or use it as a guide to write code for your preferred language.
