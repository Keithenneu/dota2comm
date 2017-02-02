import sys
from ctypes import cdll, c_char_p, c_void_p, cast


class Dota2Comm:
    def __init__(self, name):
        self.__dota2comm = cdll.LoadLibrary("dota2comm.dll")
        self.name = name
        self.__name = name.encode()
        
        self.__receiveMessage = self.__dota2comm.receiveMessage
        self.__receiveMessage.argtypes = [c_char_p]
        self.__receiveMessage.restype = c_void_p

        self.__sendMessage = self.__dota2comm.sendMessage
        self.__sendMessage.argtypes = [c_char_p, c_char_p]

        self.__freeString = self.__dota2comm.freeString
        self.__freeString.argtypes = [c_void_p]

        err = self.__dota2comm.init(self.__name)
        if err != 0:
            raise Exception("init failed! error code %d" % err)
            
        nrofclients = self.__dota2comm.getNrConnectedClients(self.__name)
        
        if nrofclients < 1:
            raise Exception("No clients found. Did the game start?")

    def receiveMessage(self):
        msg = self.__receiveMessage(self.__name)
        
        if msg is None:
            return None
        else:
            ptr = msg
            msg = cast(msg, c_char_p).value
            self.__freeString(ptr)
            return msg.decode()[:-len(self.name)]#removing r_marker

    def sendMessage(self, message):
        return self.__sendMessage(self.__name, message.encode())
        
    def exit(self):
        pass