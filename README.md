# arduino_fun

## snake game

snake game uses two libraries, 8x32 led and PS2 joystick.

![](snake/snake.gif)

## sofia catch cat

My daughter Niannian (Sofia) likes chasing a cat named Xiangbin, so we decided to create a game.
The name of the game is NNZXM, NN (Niannian) is my daughter's name, XM (Ximao) means happy cat,
and Z in the middle means chase.
We discussed how to design the game, she is the architect, and I am the software guy.

The game uses two libraries, LCD ILI9225 and PS2 joystick.
It is quite straightforward, the player will control the joystick to chase the cat, and the cat
which is controlled by the program will try to run as far as possible.

![](sofia_catch_cat/sofia_catch_cat.gif)

ino dir includes the code.
convert.py converts the tools converts my daughter and the cat's photo into a 40x40 RGB pixel
array which will be loaded into the game.