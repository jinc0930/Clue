#include "game.h"
#include "items.h"
#include "character.h"
#include "utils.h"
#include "pool.h"
#define N 9

struct Game makeGame() {
    struct Game game = {
        .attempts = 0,
        .map = { NULL },
        .items = { NULL },
        .characters = { NULL},
        .avatar = NULL,
    };

    // INIT: ROOMS, CHARACTERS, ITEMS
    for (size_t i = 0; i < N; i++) {
        game.map[i] = makeroom(ROOMS[i]);
        game.items[i] = makeitem(ITEMS[i]);
        game.characters[i] = makeroom(CHARACTERS[i]);
    }

    shuffle(game.map, N);
    shuffle(game.items, N);
    shuffle(game.characters, N);

    // ROOM CONNECTIONS
    for (int p = 0; p < N; p++ ){
        if( p > 2 ){
            setNorth(game.map[p], game.map[p-3]);
        }
        if( p < 6 ){
            setSouth(game.map[p], game.map[p+3]);
        }
        if( p != 0 && p != 3 && p != 6){
            setWest(game.map[p], game.map[p-1]);
        }
        if( p != 2 && p != 5 && p != 8){
            setEast(game.map[p], game.map[p+1]);
        }
    }

    return game;
}

// set player/avatar in the game.
// if the name exists the player 
int setPlayer(struct Game * game, char * name) {
    bool already_exists = false;
    int avatar_idx = 0;

    for (size_t i = 0; i < 9; i++) {
        if (strcasecmp(name, getcharname(game->characters[i])) == 0) {
            already_exists = true;
            avatar_idx = i;
            break;
        }
    };

    if (already_exists == false) {
        avatar_idx = rand()%9;
    }
    game->avatar = game->characters[avatar_idx];
    game->avatar->id = "avatar";
    return avatar_idx;
}