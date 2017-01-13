import os

class Dota2Comm:
    def __init__(self, path=None, **kwargs):
        if path is None:
            raise ValueError("No path to dota2 bot directory provided")
            
        self.__path = path
            
        if not os.path.exists(path + "/dota2comm/"):
            os.makedirs(path + "/dota2comm")
            
        if os.path.exists(path + "/dota2comm/rev.lua"):
            self.__rev = int(open(path + "/dota2comm/rev.lua", "r").read().split("=")[1])
        else:
            self.__rev = 0
            
    def __send(self, msg):
        self.__rev = self.__rev + 1
        messages = "messages = {\"%s\"}" % msg.replace("\\", "\\\\").replace("\"", "\\\"")

        f = open(self.__path + "/dota2comm/messages.lua", "w")
        f.write(messages)
        f.close()

        f = open(self.__path + "/dota2comm/rev.lua", "w")
        f.write("rev=" + str(self.__rev))
        f.close()

    def receiveMessage(self):
        return None

    def sendMessage(self, message):
        self.__send(message)
        return True
        
    def exit(self):
        self.__send("")