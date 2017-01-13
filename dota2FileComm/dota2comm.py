import os
import sys

path = "C:/Program Files (x86)/Steam/steamapps/common/dota 2 beta/game/dota/scripts/vscripts/bots"

if not os.path.exists(path + "/dota2comm/"):
    os.makedirs(path + "/dota2comm")
    
if os.path.exists(path + "/dota2comm/rev.lua"):
    rev = int(open(path + "/dota2comm/rev.lua", "r").read().split("=")[1])
else:
    rev = 0

def send(msg, rev):
    messages = "messages = {\"%s\"}" % msg.replace("\\", "\\\\").replace("\"", "\\\"")

    f = open(path + "/dota2comm/messages.lua", "w")
    f.write(messages)
    f.close()

    f = open(path + "/dota2comm/rev.lua", "w")
    f.write("rev=" + str(rev))
    f.close()
    
while True:
    try:
        print("> ", end="")
        msg = input()
        rev = rev + 1
        
        if msg == "exit":
            send("", rev)
            break
        
        send(msg, rev)
        
    except KeyboardInterrupt:
        break
        
    except EOFError:
        break