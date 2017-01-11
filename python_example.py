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