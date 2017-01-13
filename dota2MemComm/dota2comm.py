import sys
from ctypes import cdll, c_char_p


class Dota2Comm:
    def __init__(self, **kwargs):
        self.__dota2comm = cdll.LoadLibrary("dota2comm.dll")
        
        self.__receiveMessage = self.__dota2comm.receiveMessage
        self.__receiveMessage.restype = c_char_p

        self.__sendMessage = self.__dota2comm.sendMessage
        self.__sendMessage.argtypes = [c_char_p]

        err = self.__dota2comm.init()
        if err != 0:
            raise Exception("init failed! error code %d" % err)
            
        nrofclients = self.__dota2comm.getNrConnectedClients()
        
        if nrofclients < 1:
            raise Exception("No clients found. Did the game start?")

    def receiveMessage(self):
        msg = self.__receiveMessage()
        if msg is None:
            return None
        else:
            return msg.decode()

    def sendMessage(self, message):
        return self.__sendMessage(message.encode())
        
    def exit(self):
        pass