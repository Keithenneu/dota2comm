import os

class Dota2Comm:
    def __init__(self, name):
        if os.path.exists("path.txt"):
            self.__path = open("path.txt", "r").read()
        else:
            self.__path = input("Enter the Dota2 bot script directory: ")
            f = open("path.txt", "w")
            f.write(self.__path)
            f.close()

        print("Using path " + self.__path)

        self.__name = name

        if not os.path.exists(self.__path + "/dota2comm/"):
            os.makedirs(self.__path + "/dota2comm")

        if os.path.exists(self.__path + "/dota2comm/rev_%s.lua" % name):
            self.__rev = int(open(self.__path + "/dota2comm/rev_%s.lua" % name, "r").read().split("=")[1])
        else:
            self.__rev = 0

    def __send(self, msg):
        self.__rev = self.__rev + 1
        messages = "messages = {\"%s\"}" % msg.replace("\\", "\\\\").replace("\"", "\\\"")

        f = open(self.__path + "/dota2comm/messages_%s.lua" % self.__name, "w")
        f.write(messages)
        f.close()

        f = open(self.__path + "/dota2comm/rev_%s.lua" % self.__name, "w")
        f.write("rev=" + str(self.__rev))
        f.close()

    def receiveMessage(self):
        return None

    def sendMessage(self, message):
        self.__send(message)
        return True

    def exit(self):
        self.__send("")
