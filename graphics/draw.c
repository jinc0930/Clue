#include "raylib.h"
#include "../game.h"

void DrawMap(struct Room* map[9]) {
    int xStep = 800 / 3;
    int yStep = 450 / 3;
    int marginLeft = 80;
    int marginTop = 80;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            DrawText(TextFormat(map[(i*3)+j]->name), marginLeft+j*xStep, marginTop+i*yStep, 20, BLACK);
        }
    }
}