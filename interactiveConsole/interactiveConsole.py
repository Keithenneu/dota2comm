import sys
from threading import Thread
from time import sleep

from dota2comm import Dota2Comm

class InteractiveConsole():
    def __init__(self, name):
        self.name = name
        self.dota = Dota2Comm(self.name)
    
    def StartThreads(self):
        self.running = True
        self.threadrecv = Thread(target=self.waitForAnswers)
        self.threadrecv.start()
        self.threadsend = Thread(target=self.sendRequests)
        self.threadsend.start()
    
    def waitForAnswers(self):
        while self.running:
            msg = self.dota.receiveMessage()
            if msg is not None:
                print("%s: [Out] %s" % (self.name,str(msg)))
            sleep(0.05)
    
    def sendRequests(self):
        while self.running:
            try:
                print("%s: [In] " % (self.name), end="")
                msg = input()
                if msg == "exit":
                    self.dota.exit()
                    self.running = False
                    self.threadrecv.join()
                else:
                    self.dota.sendMessage(msg)
                sleep(0.1)
            except:
                self.running = False
                sys.exit()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: ./interactiveConsole.py marker")
        sys.exit()
    console = InteractiveConsole(sys.argv[1])
    console.StartThreads()