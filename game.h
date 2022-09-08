#include <stdio.h>
#include <stdlib.h>
#include "character.h"
#include "items.h"
#ifndef GAME_H
#define GAME_H

struct Game {
    int attempts;
    struct Character* characters[9];
    struct Item* items[9];
    struct Room* map[9];
};

struct Game makeGame();

#endif