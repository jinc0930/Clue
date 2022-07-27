# Clue
A game

To play this game you need to compile all the c files which includes: adventure.c, rooms.c , character.c and items.c
To compile, gcc all the mentioned documents above and optionally flag with -o to name the combined file
For example: gcc adventure.c rooms.c character.c items.c -o a.out
The above code will compile a combined file a.out, to run the file, simply type ./a.out in the terminal

In this game, the board consists of nine rooms, each has four pointers to it's north, south, east and west. If there is
no room in one of the directions, that pointer will point to null. The nine rooms are set on a 3*3 grid, with their name
randomnized using an array and a random number generator. Each room has a linked list called itemList to keep track of
items in that room. 

There are a total of six items and five characters(besides the player's avatar) in the game, they will spawn into their
designated rooms at the beginning of the game. Each item consist of a name and a pointer called next while each
character has name, inventory(a linked list of items), and location(indicating the room it's in).

All required commands are implemented and functional including "help","list","go","take","drop","inventory", and "clue".
The game uses scanf() to gather user input, and allow commands with secondary inputs. Players can enter a direction
after the "go" command was inputed, name of an item after "drop" and "take", and a character name (suspected murderer)
after calling "clue". If the user did not input a valid command, the game allows the user to re-enter a command without
consequences.

The game begins by asking user to input their name, and sets given name as the player's avatar name.
Then, it asks for user to input commands for gameplay.
The game will end when the user wins or loses, the user can win by guessing the answer correctly, which consist of a 
random room, random item, and a random character. If the user fails to guess all three correctly with one clue call,
when clue is called at the eleventh time, the game will end.
