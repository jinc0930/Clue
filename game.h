#include <stdio.h>
#include <stdlib.h>
#include "character.h"
#include "items.h"
#include "rooms.h"
#ifndef GAME_H
#define GAME_H

// struct for keeping track of state of the game
struct Game {
    int attempts;
    struct Character *characters[9];
    struct Item *items[9];
    struct Room *map[9];
    struct Character *avatar;
    const char* targetChar;
    const char* targetRoom;
    const char* targetItem;
    bool okChar;
    bool okRoom;
    bool okItem;
};

enum direction { North, East, West, South };

// create the game
struct Game makeGame();

// set a player
int initGame(struct Game * game, char * name);

// player move
int move(struct Game * game, enum direction dir);

enum actionResult { Invalid, NotFound, Full, Ok };
// player take
enum actionResult take(struct Game * game, char * itemName);
// player drop
enum actionResult drop(struct Game * game, char * itemName);
// call clue
enum actionResult clue(struct Game * game, char * murderer);

// free everything
void freeGame(struct Game * game);
#endif