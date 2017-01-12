import sys
from threading import Thread
from time import sleep

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
nroflients = dota2comm.getNrConnectedClients()

if nroflients < 1:
    print("No clients found. Is the game running?")
    sys.exit()

def waitForAnswers():
    while running:
        msg = receiveMessage()
        if msg is not None:
            print("[Out] %s" % str(msg.decode()))
        sleep(0.05)
    
running = True
    
thread = Thread(target=waitForAnswers)
thread.start()
    
while running:
    try:
        print("[In] ", end="")
        msg = input()
        if msg == "exit":
            running = False
            thread.join()
        else:
            sendMessage(msg.encode())
        sleep(0.1)
    except:
        running = False
        sys.exit()