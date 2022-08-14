# Clue

To play this game you need to compile all the c files which includes: adventure.c, rooms.c , character.c, items.c, util.c, prettyprint.c, and pool.c
To compile, gcc all the mentioned documents above and optionally flag with -o to name the combined file
For example: gcc utils.c prettyprint.c pool.c character.c rooms.c items.c adventure.c -o a.out
The above code will compile a combined file a.out, to run the file, simply type ./a.out in the terminal

In this game, the board consists of nine rooms, each has four pointers to it's north, south, east and west. If there is
no room in one of the directions, that pointer will point to null. The nine rooms are set on a 3*3 grid, with their name
randomnized with the defined structure pool, which manipulates arrays and random number generators. Each room has a linked list called itemList to keep track of all items in that room. 

There are a total of nine items and nine characters(including the player's avatar) in the game, they will spawn into random rooms at the beginning of the game. Each item consist of a name and a pointer called next while each
character has name, inventory(a linked list of items), and location(indicating the room it's in).
The 8 npcs, however, are tagged with an ID, they can either be a "hint giver" or an "accuser", and ofcourse, one of the npcs will be the "murderer". The "hint givers" will give the user three guarenteed accurate hints (one about each category: room, item, person) that eliminates the suspicsion. The "accuser" will give the user three accusations (one of each category) that are not guarenteed to be true, the chance of it being true varies from one accuser to another. The "murderer" accuses things like an accuser, but is guranteed to be incorrect.

Commands include "help","list","go","take","drop","inventory","look","talk", and "clue".
The game uses scanf() to gather user input, and allow commands with secondary inputs. Players can enter a direction
after the "go" command was inputed, or name of an item after "drop" and "take", and a character name (suspected murderer)
after calling "clue". To accomodate user habits, commands can be entered in one line or two separate lines. Users can type "go south" or "go" *enter* "south". If the user did not input a valid command, the game allows the user to re-enter a command without
consequences.

The game begins by asking user to input their name, and sets given name as the player's avatar name.
Then, it asks for user to input commands for gameplay.
The game will end when the user wins or loses, the user can win by guessing the answer correctly, which consist of a 
random room, random item, and a random character. The player can keep guessing until clue is called at the eleventh time, where the game will end.
