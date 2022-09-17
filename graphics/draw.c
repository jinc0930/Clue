#include "raylib.h"
#include "../game.h"
#include "../items.h"

void DrawMap(struct Room* map[9], Texture2D char_textures[], int _x, int _y, int width, int height) {
    int xStep = (width / 3) - 5;
    int yStep = (height / 3) - 5;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            int idx = (i*3)+j;
            int x = _x + (j)*xStep + 10.0f * j;
            int y = _y + (i)*yStep + 10.0f * i;

            char_textures[0].width = 60;
            char_textures[0].height = 60;
            // characters

            Rectangle rec = { .width = xStep, .height = yStep, .x = x, .y = y };
            DrawRectangleRec(rec, SKYBLUE);
            DrawText(TextFormat(map[idx]->name), x, y, 20, BLACK);
            DrawTexture(char_textures[0], x, y + yStep - 60, WHITE);
        }
    }
}

void DrawItems(struct Room* room, int _x, int _y) {
    struct Item *item = room->itemList;
    int step = 1;

    DrawText(TextFormat("Items"), _x, _y, 29, BLACK);
    
    while (item != NULL) {
        int x = _x ;
        int y = _y + 40.0f * step++;

        DrawText(TextFormat(item->name), x, y, 20, BLUE);
        item = item->next;
    }
}