#include "raylib.h"
#include "../game.h"
#include "../items.h"
#include "../character.h"
#include "../rooms.h"
#include <stdbool.h>

#ifndef DRAW_H
#define DRAW_H
typedef enum GameScreen { START = 0, GAMEPLAY, CHEATSHEET, ENDING } GameScreen;

struct ChatState {
    bool is_open;
    int talking_to;
    int part;
};

// Draw map
void DrawMap(struct Room* map[9], Texture2D char_textures[], int _x, int _y, int width, int height) ;

// Draw items
void DrawItems(struct Room * map, int _x, int _y);

// Draw Cheatsheet
void DrawCheatSheet(bool items[N_ITEMS], Vector2 mousePoint) ;

void DrawChat(int _x, int _y, int width);

#endif