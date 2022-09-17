#include "raylib.h"
#include "../game.h"
#include "../items.h"
#include "../character.h"
#include "../rooms.h"

#ifndef DRAW_H
#define DRAW_H

// Draw map
void DrawMap(struct Room* map[9], Texture2D char_textures[], int _x, int _y, int width, int height) ;

// Draw items
void DrawItems(struct Room * map, int _x, int _y);

#endif