# Dota2MemComm

This backend utilises the WinApi to read and write into Dota's memory.

__The major downside:__
 It's messing with the memory of Dota2.
It focuses on the Lua VM only, but Valve might still not like/allow this. If they tell me to do so, I'll take this down immediately.

__VAC:__
I don't know if the Valve Anti Cheat system is able to catch this program. I doubt it. But you have been warned.

Sorry for my bad C/C++, I'm not a native speaker.

## Build
You have to build the DLL first. I used Dev-Cpp (make sure to enable C++11 like [this](http://stackoverflow.com/questions/16951376/how-to-change-mode-from-c98-mode-in-dev-c-to-a-mode-that-supports-c0x-ran)).
But you can use any C++11 compliant compiler that can build DLLs.

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

### Outside part (Python3 for example)

(python_example.py)

````python
    import sys

    # we'll be using ctypes to handle the dll
    from ctypes import cdll, c_char_p

    # load the dll
    dota2comm = cdll.LoadLibrary("dota2comm.dll")

    # set some types
    receiveMessage = dota2comm.receiveMessage
    receiveMessage.restype = c_char_p

    sendMessage = dota2comm.sendMessage
    sendMessage.argtypes = [c_char_p]

    # initialize the dll and connect to lua
    err = dota2comm.init()
    if err != 0:
    	print("init failed! error code", err);
    	sys.exit()

    # Print the number of connected Lua modules
    print("Got %d clients." % dota2comm.getNrConnectedClients())

    # Receive a message
    message = receiveMessage()

    # Send a message
    succes = sendMessage(message)
````

## Security considerations

__The lua part itself can't do anything.__ It just isn't able to do anything, because of how the Dota sandbox works.

Because of that, distributing the Lua part of a bot poses no security threat to anyone.

The outside part however is just any program someone wrote. And, just like any other program in the world, might be dangerous.

## Current state

This code is in an *works on my machine* state. Meaning, that it somewhat runs under Windows 10 64bit.

## TODO

-   Do some proper testing
-   Create some more convenience functions
-   Let the user specify the buffer size
-   Add a way to connect to certain Lua modules only (Radiant and Dire are both connected at the moment)
-   Make the C++ code look less ugly
