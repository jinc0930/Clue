#include "raylib.h"
#include "../game.h"
#include "../items.h"
#include "../rooms.h"
#include "../character.h"
#include "./draw.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
const int keys[] = { KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE, KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE };
const int space = 8;

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
            DrawText(TextFormat(map[idx]->name), x + space, y + space, 20, BLACK);
            for (size_t i = 0; i < MAX_CHARACTER; i++) {
                if (map[idx]->chara[i] == NULL) continue;
                int offset = char_size * i + space * i;
                if (strcmp(map[idx]->chara[i]->id, "avatar") == 0) {
                    texture = 1;
                } else texture = 0;
                char_textures[texture].width = char_size;
                char_textures[texture].height = char_size;
                DrawTexture(char_textures[texture], x + offset + space, y + yStep - char_size - space, WHITE);
            }
        }
    }
}

void DrawSide(struct Game * game, int x, int y) {
    DrawText("Shortcut:", x, y, 10, BLACK);
    DrawText("[C] to open cheatsheet", x, y + 20, 10, DARKGRAY);
    DrawText("[Arrow] to move", x, y + 40, 10, DARKGRAY);
    DrawText("Inventory", x, space, 20, BLACK);

    struct Item * inventory = game->avatar->inventory;
    int i = 0;
    while (inventory != NULL) {
        DrawText(inventory->name, x, i++ * 20 + space *2 + 20, 20, BLUE);
        inventory = inventory->next;
    }
}

void DrawCheatSheet(bool items[N_ITEMS], Vector2 mousePoint) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    int step = 0;
    DrawText("Press C to close", GetScreenWidth() - 200, GetScreenHeight() - 20 - space, 20, DARKGRAY);
    for (int i = 0; i < N_ITEMS; i++) {
        int y = 30.0f * step++ + space;
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

static void DrawPressEnter() {
    DrawText("[ENTER]", GetScreenWidth() - 100, GetScreenHeight() - 20 - space, 20, DARKGRAY);
}

static void DrawChat(FullState * state, int height) {
    struct Game * game = state->game;
    struct Room * current_room = game->avatar->location;
    ChatState * chat = &state->chat;
    
    int y = GetScreenHeight() - height + space;
    if (chat->talking_to != NULL) {
        WordWrap(TextFormat("- %s %s.", chat->talking_to->prefix[chat->page], chat->talking_to->hints[chat->page]), 70, space, y, 20, WHITE);
            DrawPressEnter();
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D)) {
            if (chat->page + 1 < MAX_CHARACTER) chat->page += 1;
            else (chat->page = 0, chat->talking_to = NULL, state->lock = 0);
        }
    } else {
        DrawText("Who do you want to talk to?", space, y, 20, SKYBLUE);
        int prev = 0;
        for (int i = 0; i < MAX_CHARACTER; i++) {
            struct Character * c = current_room->chara[i];
            if (c != NULL && strcmp(c->id, "avatar") != 0) {
                if (i == 0) {
                    if (IsKeyPressed(KEY_ENTER)) chat->talking_to = c;
                }
                if (IsKeyPressed(keys[i])) chat->talking_to = c;
                const char * text = TextFormat("%d. %s", i + 1, c->name);
                DrawText(text, space + (space*4) * i + prev, y + 20 + space*2, 20, WHITE);
                prev += MeasureText(text, 20);
            }
        }
    }
}

static void DrawTake(FullState * state, int height) {
    struct Room * current_room = state->game->avatar->location;
    int y = GetScreenHeight() - height + space;
    struct Item *item = current_room->itemList;
    int step = 0, prev = 0;
    DrawText("Take item:", space, y, 20, SKYBLUE);
    while (item != NULL) {
        if (IsKeyPressed(keys[step])) {
            switch (take(state->game, item->name)) {
            case Ok:
                break;
            case Full:
                state->bottom_screen = BSCREEN_ERROR;
                strncpy(state->error, "sorry, your inventory is full", MAX_ERROR_CHARS); 
                break;
            case NotFound:
                state->bottom_screen = BSCREEN_ERROR;
                strncpy(state->error, "sorry, there is nothing in this room", MAX_ERROR_CHARS); 
                break;
            case Invalid:
                state->bottom_screen = BSCREEN_ERROR;
                strncpy(state->error, "that item does not exist", MAX_ERROR_CHARS); 
                break;
            };
        };
        const char * text = TextFormat("%d. %s", step + 1, item->name);
        DrawText(text, space + (space*4) * step++ + prev, y + 20 + space*2, 20, WHITE);
        prev += MeasureText(text, 20);
        item = item->next;
    }
    if (step == 0) DrawText("Empty..", space + prev, y + 20 + space*2, 20, GRAY);
}

static void DrawDrop(struct Game * game, int height) {
    int y = GetScreenHeight() - height + space;
    struct Item *item = game->avatar->inventory;
    int step = 0, prev = 0;
    DrawText("Drop item:", space, y, 20, SKYBLUE);
    while (item != NULL) {
        if (IsKeyPressed(keys[step])) {
            enum actionResult result = drop(game, item->name);
            if (result == Ok) break;
        }
        const char * text = TextFormat("%d. %s", step + 1, item->name);
        DrawText(text, space + (space*4) * step++ + prev, y + 20 + space*2, 20, WHITE);
        prev += MeasureText(text, 20);
        item = item->next;
    }
    if (step == 0) DrawText("Empty..", space + prev, y + 20 + space*2, 20, GRAY);
}

static void DrawIdle(struct InputState * input_state, BottomScreen * bottom_screen, int height) {
    int y = GetScreenHeight() - height + space;
    DrawText("Choose an action:", space, y, 20, SKYBLUE);
    char * arr[] = {"Talk", "Take", "Drop", "Clue"};
    int len = sizeof(arr)/sizeof(arr[0]);
    int prev = 0, step = 0;
    for (int i = 0; i < len; i++) {
        const char * text = TextFormat("%d. %s", step + 1, arr[i]);
        DrawText(text, space + (space*4) * step++ + prev, y + 20 + space*2, 20, WHITE);
        prev += MeasureText(text, 20);
    }

    if (IsKeyPressed(keys[0])) *bottom_screen = BSCREEN_TALK;
    else if (IsKeyPressed(keys[1])) *bottom_screen = BSCREEN_TAKE;
    else if (IsKeyPressed(keys[2])) *bottom_screen = BSCREEN_DROP;
    else if (IsKeyPressed(keys[3])) {
        *bottom_screen = BSCREEN_CLUE;
        input_state->count = 0;
        input_state->text[0] = '\0';
    };
}


static void DrawTextInput(struct InputState * input_state, Rectangle box, int frames_count, char * dest) {
    DrawRectangleRec(box, BLACK);
    DrawRectangleLines(box.x, box.y, box.width, box.height, LIGHTGRAY);
    DrawText(dest, box.x + 5, box.y + 8, 20, WHITE);
    int key = GetCharPressed();
    if (isalpha(key) && (input_state->count < MAX_INPUT_CHARS)) {
        input_state->text[input_state->count] = (char)key;
        input_state->text[input_state->count+1] = '\0';
        input_state->count++;
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        input_state->count--;
        if (input_state->count < 0) input_state->count = 0;
        input_state->text[input_state->count] = '\0';
    }
    if (input_state->count < MAX_INPUT_CHARS) {
        // Draw blinking underscore char
        if (((frames_count/20)%2) == 0) DrawText("_", (int)box.x + 8 + MeasureText(input_state->text, 20), (int)box.y + 12, 20, SKYBLUE);
    }
    DrawPressEnter();
    DrawText(TextFormat("%i/%i", input_state->count, MAX_INPUT_CHARS), box.x, box.y + 28 + space, 10, GRAY);
}

static void DrawClue(FullState * state, int height) {
    int y = GetScreenHeight() - height + space;
    DrawText("Who is the murderer?", space, y, 20, WHITE);
    DrawTextInput(&state->input_state, (Rectangle){space, y + 20 + space, 220, 30}, state->frames_counter, state->input_state.text);

    if (IsKeyPressed(KEY_ENTER)) {
        switch (clue(state->game, state->input_state.text)) {
        case Full:
            state->bottom_screen = BSCREEN_ERROR;
            strncpy(state->error, "too many characters in this room already", MAX_ERROR_CHARS); 
            break;
        case Ok:
            state->bottom_screen = BSCREEN_CLUE_POST;
            break;
        default:
            state->bottom_screen = BSCREEN_ERROR;
            strncpy(state->error, "not a valid character", MAX_ERROR_CHARS); 
            break;
        };
    }
}

static void DrawCluePost(struct Game * game, BottomScreen * bottom_screen, struct InputState * input_state, int height) {
    int y = GetScreenHeight() - height + space;
    const char * c1 = "Room: ";
    DrawText(c1, space, y, 20, WHITE);
    DrawText(game->okRoom ? "OK" : "WRONG", space + MeasureText(c1, 20), y, 20, game->okRoom ? GREEN : RED);

    const char * c2 = "Item: ";
    DrawText(c2, space, y + 20 + space, 20, WHITE);
    DrawText(game->okRoom ? "OK" : "WRONG", space + MeasureText(c2, 20), y + 20 + space, 20, game->okRoom ? GREEN : RED);

    const char * c3 = "Murderer: ";
    DrawText(c3, space, y + 40 + space * 2, 20, WHITE);
    DrawText(game->okRoom ? "OK" : "WRONG", space + MeasureText(c3, 20), y + 40 + space * 2, 20, game->okRoom ? GREEN : RED);
    
    DrawPressEnter();
}

static void DrawError(const char * txt, int height) {
    int y = GetScreenHeight() - height + space;
    DrawText(txt, space, y, 20, RED);
    DrawPressEnter();
}

void DrawBottomScreen(struct FullState * state, int height) {
    DrawRectangle(0, GetScreenHeight() - height, GetScreenWidth(), height, BLACK);
    BottomScreen * screen = &state->bottom_screen;
    switch(*screen) {
        case BSCREEN_IDLE: {
            if (state->lock != 0) state->lock = 0;
            DrawIdle(&state->input_state, screen, height);
        } break;
        case BSCREEN_TALK: {
            DrawChat(state, height);
        } break;
        case BSCREEN_TAKE: {
            DrawTake(state, height);
            if (IsKeyPressed(KEY_ENTER)) *screen = BSCREEN_IDLE;
        } break;
        case BSCREEN_DROP: {
            DrawDrop(state->game, height);
            if (IsKeyPressed(KEY_ENTER)) *screen = BSCREEN_IDLE;
        } break;
        case BSCREEN_CLUE: {
            DrawClue(state, height);
        } break;
        case BSCREEN_CLUE_POST: {
            if (state->lock != 0) state->lock = 0;
            DrawCluePost(state->game, screen, &state->input_state, height);
            if (IsKeyPressed(KEY_ENTER)) {
                 *screen = BSCREEN_IDLE;
            }
        } break;
        case BSCREEN_ERROR: {
            if (state->lock != 0) state->lock = 0;
            DrawError(state->error, height);
            if (IsKeyPressed(KEY_ENTER)) {
                state->error[0] = "\0";
                *screen = BSCREEN_IDLE;
            }
        } break;
    }
}
