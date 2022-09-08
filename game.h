#include <stdio.h>
#include <stdlib.h>
#include "character.h"
#include "items.h"
#include "rooms.h"
#ifndef GAME_H
#define GAME_H

static const char* CHARACTERS[] = {"minh","james","ivan","tenzin","edrick","chang","kevin","michael","joey"};
static const char* ITEMS[] = {"butter knife","bat","wrench","rope","dagger","rifle","hammer","lead pipe","poison bottle"};
static const char* ROOMS[] = {"kitchen","hall","study","bathroom","ballroom","library","lounge","office","bedroom"};

// struct for keeping track of state of the game
struct Game {
    int attempts;
    struct Character *characters[9];
    struct Item *items[9];
    struct Room *map[9];
    struct Character *avatar;
};

// create the game
struct Game makeGame();

// set a player
int setPlayer(struct Game * game, char * name);

#endif