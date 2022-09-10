# Clue
## How to compile
1. You need to install [raylib](https://www.raylib.com), make and a c99 compiler like gcc. You can skip all of this in case you already have it by using MSYS2 or any other way. For unix systems: [raylib on macOs](https://github.com/raysan5/raylib/wiki/Working-on-macOS), [raylib on linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

2. Clone this repository and execute `mingw32-make` or just `make` command in the terminal.

3. Some adjustments in the `Makefile` may be needed depending where you installed raylib. I recommend you changing the `RAYLIB_PATH` and `COMPILER_PATH` variables in the `Makefile` accordingly.

4. If the compilation is successfull now you can execute the output file "clue.exe" in the same directory. To run the file, simply type ./clue.exe in the terminal.

## How to play
todo

## How to test
1. Compile by executing `mingw32-make` or just `make`
2. Execute: `tests/output`

# Clue legacy
The legacy is the terminal text-based way of playing the game.

## Compile the legacy version
For the legacy version you don't need to have raylib installed. But you still need a compiler. To compile enter the legacy folder and execute make command in the terminal.

## How to play the legacy

In this game, the board consists of nine rooms, each has four pointers to it's north, south, east and west. If there is
no room in one of the directions, that pointer will point to null. The nine rooms are set on a 3*3 grid, with their name
randomnized with the defined structure pool, which manipulates arrays and random number generators. Each room has a linked list called itemList to keep track of all items in that room. 

There are a total of nine items and nine characters(including the player's avatar) in the game, they will spawn into random rooms at the beginning of the game. Each item consist of a name and a pointer called next while each
character has name, inventory(a linked list of items), and location(indicating the room it's in).
The 8 npcs, however, are tagged with an ID, they can either be a "hint giver" or an "accuser", and of course, one of the npcs will be the "murderer". The "hint givers" will give the user three guarenteed accurate hints (one about each category: room, item, person) that eliminates the suspicsion. The "accuser" will give the user three accusations (one of each category) that are not guarenteed to be true, the chance of it being true varies from one accuser to another. The "murderer" will blend in and act either as a "hint giver" or an "accuser".

Commands include "help","list","go","take","drop","inventory","look","talk", and "clue".
The game uses scanf() to gather user input, and allow commands with secondary inputs. Players can enter a direction
after the "go" command was inputed, or name of an item after "drop" and "take", and a character name (suspected murderer)
after calling "clue". To accomodate user habits, commands can be entered in one line or two separate lines. Users can type "go south" or "go" *enter* "south". If the user did not input a valid command, the game allows the user to re-enter a command without
consequences.

The game begins by asking user to input their name, and sets given name as the player's avatar name.
Then, it asks for user to input commands for gameplay.
The game will end when the user wins or loses, the user can win by guessing the answer correctly, which consist of a 
random room, random item, and a random character. The player can keep guessing until clue is called at the eleventh time, where the game will end.
