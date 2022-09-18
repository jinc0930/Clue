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
    struct Character * talking_to;
    int count;
    int page;
};

// Draw map
void DrawMap(struct Room* map[9], Texture2D char_textures[], int _x, int _y, int width, int height) ;

// Draw items
void DrawItems(struct Room * map, int _x, int _y);

// Draw Cheatsheet
void DrawCheatSheet(bool items[N_ITEMS], Vector2 mousePoint) ;

void DrawChat(struct Room * current_room, struct ChatState * chat, int _x, int _y, int width, int height);

#endif