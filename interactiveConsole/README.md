# Interactive Console
This is a interactive Console, which can execute code in a bots context.

## Usage

First, decide on a communication backend. Read the respective README files for help, or the main README for an overview.

#### Memory backend

Put the the bots folder content in your dota installation's bot folder, together with the `dota2comm.lua` file.
Put `interactiveConsole.py` somewhere, together with `dota2comm.py` and the compiled `dota2comm.dll`.

Start a lobby game with the bots.

Run `python3 interactiveConsole.py "name of your client"` and start typing some stuff.

#### File backend

Put the the bots folder content in your dota installation's bot folder, together with the `dota2comm.lua` file.
Put `interactiveConsole.py` somewhere, together with `dota2comm.py`.

Start a lobby game with the bots.

Run `python3 interactiveConsole.py "path/to/your/bots/directory"` and start typing some stuff.

Make sure your dota console is open, if you want to see the responses.

Exit with `exit` to make sure it exits gracefully

## What it looks like

(Using the memory backend)

![game](../../images/dota2_comm_multi_bots.png)

#### Things you can do in the console

![console](../../images/console.png)
