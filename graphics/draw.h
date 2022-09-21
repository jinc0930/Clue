#include "raylib.h"
#include "../game.h"
#include "../items.h"
#include "../character.h"
#include "../rooms.h"
#include <stdbool.h>

#ifndef DRAW_H
#define DRAW_H

typedef enum GameScreen { START = 0, GAMEPLAY, CHEATSHEET, ENDING } GameScreen;
typedef enum BottomScreen { IDLE, TALK, TAKE, DROP, CLUE } BottomScreen;

struct ChatState {
    struct Character * talking_to;
    int count;
    int page;
};

// Draw map
void DrawMap(struct Room* map[9], Texture2D char_textures[], int _x, int _y, int width, int height);

// Draw instructions
void DrawSide(int x, int y);

/*
 * SCREENS
 */

// Draw Cheatsheet
void DrawCheatSheet(bool items[N_ITEMS], Vector2 mousePoint) ;

// Draw Bottom Screen
void DrawBottomScreen(struct Game * game, BottomScreen * bottom_screen, struct ChatState * chat, int height);

#endif