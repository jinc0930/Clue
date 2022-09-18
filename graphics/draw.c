#include "raylib.h"
#include "../game.h"
#include "../items.h"
#include "../rooms.h"
#include "../character.h"
#include "./draw.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

inline int wordlen(const char * str){
   int idx = 0;
   while(str[idx]!=' ' && str[idx]!=0 && str[idx]!='\n'){
      ++idx;
   }
   return idx;
}

void WordWrap(const char * src, const int max_width, int x, int y, int fontSize, Color color) {
   char dest[128] = {0};
   int idx = 0;
   int line = 0;

   strncpy(dest, src, 128);
   while(dest[idx] != '\0'){
      if(dest[idx] == '\n'){
         line = 0;
      } else if(dest[idx] == ' '){
         if(line+wordlen(&dest[idx+1]) >= max_width){
            dest[idx] = '\n';
            line = 0;
         }
      }
      line++;
      idx++;
   }

   DrawText(dest, x, y, fontSize, color);
}


void DrawMap(struct Room* map[9], Texture2D char_textures[], int _x, int _y, int width, int height) {
    int xStep = (width / 3) - 5;
    int yStep = (height / 3) - 5;
    int char_size = 50;
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            int idx = (i*3)+j;
            int x = _x + (j)*xStep + 10.0f * j;
            int y = _y + (i)*yStep + 10.0f * i;
            int texture = 0;
            Rectangle rec = { .width = xStep, .height = yStep, .x = x, .y = y };
            DrawRectangleRec(rec, SKYBLUE);
            DrawText(TextFormat(map[idx]->name), x, y, 20, BLACK);
            for (size_t i = 0; i < MAX_CHARACTER; i++) {
                if (map[idx]->chara[i] == NULL) continue;
                int offset = char_size * i + 8 * i;
                if (strcmp(map[idx]->chara[i]->id, "avatar") == 0) {
                    texture = 1;
                }
                char_textures[texture].width = char_size;
                char_textures[texture].height = char_size;
                DrawTexture(char_textures[texture], x + offset, y + yStep - char_size, WHITE);
            }
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

void DrawCheatSheet(bool items[N_ITEMS], Vector2 mousePoint) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    int step = 1;
    DrawText("Press C to close", GetScreenWidth() - 200, GetScreenHeight() - 20, 20, DARKGRAY);
    for (int i = 0; i < N_ITEMS; i++) {
        int y = 40.0f * step++;
        DrawText(CHARACTERS[i], 40, y, 20, BLACK);
        Rectangle btnBounds = { 20, y, 100, 20 };
        if (CheckCollisionPointRec(mousePoint, btnBounds)){
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) items[i] = !items[i];
        }
        DrawRectangle(10, y, 20, 20, DARKGRAY);
        if (items[i] == false) {
            DrawRectangle(12, y + 2, 16, 16, WHITE);
        }
    }
}

const int keys[] = { KEY_ONE, KEY_TWO, KEY_THREE };

void DrawChat(struct Room * current_room, struct ChatState * chat, int _x, int _y, int width, int height) {
    int m_top = 8;
    int m_left = 8;
    int y = GetScreenHeight() - height;
    DrawRectangle(0, GetScreenHeight() - height, GetScreenWidth(), height, BLACK);

    if (chat->talking_to != NULL) {
        char text[200] = { 0 };
        sprintf(text, "- %s %s.\n", chat->talking_to->prefix[chat->page], chat->talking_to->hints[chat->page]);
        WordWrap(text, 70, m_left, y + m_top, 20, WHITE);
        DrawText("[ENTER]", GetScreenWidth() - 100, GetScreenHeight() - 28, 20, DARKGRAY);

        if (IsKeyPressed(KEY_ENTER)) {
            if (chat->page + 1 < MAX_CHARACTER) chat->page += 1;
            else (chat->page = 0, chat->talking_to = NULL);
        }
    } else {
        DrawText("Who do you want to talk to?", m_left, y + m_top, 20, WHITE);
        char text[100] = {0};
        for (int i = 0; i < MAX_CHARACTER; i++) {
            struct Character * c = current_room->chara[i];
            if (c != NULL && strcmp(c->id, "avatar") != 0) {
                if (IsKeyPressed(keys[i])) chat->talking_to = c;
                char temp[32] = {0};
                sprintf(temp, "%d. %s\n", i+1, c->name);
                strncat(text, temp, 32);
            }
        }
        DrawText(text, m_left, y + m_top + 24, 20, WHITE);
    }
}
