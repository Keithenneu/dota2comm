import sys
from threading import Thread
from time import sleep

from dota2comm import Dota2Comm

if len(sys.argv) == 2:
    dota = Dota2Comm(path=sys.argv[1])
else:
    dota = Dota2Comm()

def waitForAnswers():
    while running:
        msg = dota.receiveMessage()
        if msg is not None:
            print("[Out] %s" % str(msg))
        sleep(0.05)
    
running = True
    
thread = Thread(target=waitForAnswers)
thread.start()
    
while running:
    try:
        print("[In] ", end="")
        msg = input()
        if msg == "exit":
            dota.exit()
            running = False
            thread.join()
        else:
            dota.sendMessage(msg)
        sleep(0.1)
    except:
        running = False
        sys.exit()