#include <stdio.h>
#include <stdlib.h>
#include "character.h"
#include "items.h"
#include "rooms.h"
#ifndef GAME_H
#define GAME_H
#define N_CHARACTERS 9
#define N_ROOMS 9
#define N_ITEMS 9
#define N_SPECIAL_ITEMS 2
#define MAX_ATTEMPTS 9

// struct for keeping track of state of the game
struct Game {
    int attempts;
    struct Character *characters[N_CHARACTERS];
    struct Item *items[N_ITEMS];
    struct Item *special[N_SPECIAL_ITEMS];
    struct Room *map[N_ROOMS];
    struct Character *avatar;
    const char* targetChar;
    const char* targetRoom;
    const char* targetItem;
    bool okChar;
    bool okRoom;
    bool okItem;
    bool finished;
    bool started;
    int replacedChar;
};

extern const char* CHARACTERS[];
extern const char* ITEMS[];
extern const char* ROOMS[];

enum direction { North, East, West, South };

// create the game
struct Game makeGame();

// intialize the game with a name for the player
int initGame(struct Game * game, const char * name);

// player move
//-1 if is full/invalid
//-2 if is locked
// >= 0 if is ok
int move(struct Game * game, enum direction dir);

enum actionResult { Invalid, NotFound, Full, Ok };
// player take
enum actionResult take(struct Game * game, const char * itemName);
// player drop
enum actionResult drop(struct Game * game, const char * itemName);
// call clue
enum actionResult clue(struct Game * game, const char * murderer);

// player teleport used for testing.
void teleport(struct Game * game, int roomIdx);

// free everything
void freeGame(struct Game * game);

// lock room and return index of the room in the map
int lockRandomRoom(struct Game * game);
#endif