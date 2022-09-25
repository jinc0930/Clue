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
            Rectangle rec = { .width = xStep, .height = yStep, .x = x, .y = y };
            DrawRectangleRec(rec, SKYBLUE);
            DrawText(TextFormat(map[idx]->name), x + space, y + space, 20, BLACK);
            for (size_t i = 0; i < MAX_CHARACTER; i++) {
                if (map[idx]->chara[i] == NULL) continue;
                int offset = char_size * i + space * i;
                int texture = 0;
                if (strcmp(map[idx]->chara[i]->id, "avatar") == 0) {
                    texture = 9;
                } else texture = map[idx]->chara[i]->uid;
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

    int margin = 60 + space * 6;
    DrawText("Room", x, space + margin, 20, BLACK);
    int inc = 0;
    for (size_t j = 0; j < MAX_CHARACTER; j++){
        struct Character * ch = game->avatar->location->chara[j]; 
        if (ch != NULL) {
            DrawText(ch->name, x, inc++ * 20 + space *2 + 20 + margin, 20, strcmp(ch->name, game->avatar->name) == 0 ? LIGHTGRAY : BLUE);
        }
    }
}

void DrawCheatSheet(struct FullState * state) {
    int margin_top = 60;
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    int step = 0;
    DrawText("CHEATSHEET", space, space * 2, 20, DARKGRAY);
    DrawText("Press C to close", GetScreenWidth() - 200, GetScreenHeight() - 20 - space, 20, DARKGRAY);
    int remove = 0;
    for (int i = 0; i < N_CHARACTERS; i++) {
        int y = 30 * step++ + space - remove + margin_top;
        int x = 0;
        if (i == state->game->replacedChar) {
            remove = 30;
            continue; // skip
        }
        DrawText(CHARACTERS[i], 40 + x, y, 20, BLACK);
        Rectangle btnBounds = { 20 + x, y, MeasureText(CHARACTERS[i], 20) + 20, 20 };
        if (CheckCollisionPointRec(state->mouse_pos, btnBounds)){
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) state->cheatsheet_characters[i] = !state->cheatsheet_characters[i];
        }
        DrawRectangle(10 + x, y, 20, 20, DARKGRAY);
        if (state->cheatsheet_characters[i] == false) {
            DrawRectangle(12, y + 2, 16, 16, WHITE);
        }
    }

    step = 0;
    for (int i = 0; i < N_ITEMS; i++) {
        int y = 30 * step++ + space + margin_top;
        int x = 240;
        DrawText(ITEMS[i], 40 + x, y, 20, BLACK);
        Rectangle btnBounds = { 20 + x, y, MeasureText(ITEMS[i], 20) + 20, 20 };
        if (CheckCollisionPointRec(state->mouse_pos, btnBounds)){
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) state->cheatsheet_items[i] = !state->cheatsheet_items[i];
        }
        DrawRectangle(10 + x, y, 20, 20, DARKGRAY);
        if (state->cheatsheet_items[i] == false) {
            DrawRectangle(12 + x, y + 2, 16, 16, WHITE);
        }
    }

    step = 0;
    for (int i = 0; i < N_ROOMS; i++) {
        int y = 30 * step++ + space + margin_top;
        int x = 240*2;
        DrawText(ROOMS[i], 40 + x, y, 20, BLACK);
        Rectangle btnBounds = { 20 + x, y, MeasureText(ROOMS[i], 20) + 20, 20 };
        if (CheckCollisionPointRec(state->mouse_pos, btnBounds)){
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) state->cheatsheet_rooms[i] = !state->cheatsheet_rooms[i];
        }
        DrawRectangle(10 + x, y, 20, 20, DARKGRAY);
        if (state->cheatsheet_rooms[i] == false) {
            DrawRectangle(12 + x, y + 2, 16, 16, WHITE);
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

static void DrawIdle(char * text_input, BottomScreen * bottom_screen, int height) {
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
        text_input[0] = '\0';
    };
}

static void DrawTextInput(char * dest, Rectangle box, Color color, Color bg, Color border, int frames_count) {
    DrawRectangleRec(box, bg);
    DrawRectangleLines(box.x, box.y, box.width, box.height, border);
    DrawText(dest, box.x + 5, box.y + 8, 20, color);
    int key = GetCharPressed();
    int len = strlen(dest);
    if (isalpha(key) && (len < MAX_INPUT_CHARS)) {
        dest[len] = (char)key;
        dest[len+1] = '\0';
    }
    if (IsKeyPressed(KEY_BACKSPACE) && len > 0) {
        dest[len-1] = '\0';
    }
    if (len < MAX_INPUT_CHARS) {
        if (((frames_count/20)%2) == 0) DrawText("_", (int)box.x + 8 + MeasureText(dest, 20), (int)box.y + 12, 20, SKYBLUE);
    }
    DrawText(TextFormat("%i/%i", len, MAX_INPUT_CHARS), box.x, box.y + 28 + space, 10, ColorAlpha(color, 0.5));
}

void DrawStart(char * text_input, int frames_count) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    DrawTextInput(text_input, (Rectangle){GetScreenWidth() / 2 - 110, GetScreenHeight() / 2 - 10, 220, 30}, BLUE, WHITE, GRAY, frames_count);
    const char * your_name = "Type your name:";
    DrawText(your_name, GetScreenWidth() / 2 - 110, GetScreenHeight() / 2 - 22, 10, GRAY);

    const char * c = "CLUE";
    DrawText(c, GetScreenWidth() / 2 - MeasureText(c, 40) / 2, GetScreenHeight() / 2 - 100, 40, BLUE);
}

void DrawEnd(FullState * state) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    if (state->game->okChar && state->game->okItem && state->game->okRoom) {
        const char * c = "YOU WIN";
        DrawText(c, GetScreenWidth() / 2 - MeasureText(c, 40) / 2, GetScreenHeight() / 2 - 100, 40, BLUE);
    } else {
        const char * c = "YOU LOSE";
        DrawText(c, GetScreenWidth() / 2 - MeasureText(c, 40) / 2, GetScreenHeight() / 2 - 100, 40, RED);
    }
    DrawPressEnter();
}

static void DrawClue(FullState * state, int height) {
    int y = GetScreenHeight() - height + space;
    DrawText("Who is the murderer?", space, y, 20, WHITE);
    DrawTextInput(state->text_input, (Rectangle){space, y + 20 + space, 220, 30}, WHITE, BLACK, GRAY, state->frames_counter);
    DrawPressEnter();

    if (IsKeyPressed(KEY_ENTER)) {
        switch (clue(state->game, state->text_input)) {
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
    
    if (state->game->finished) {
        state->lock = LOCK_KEYS | LOCK_MOVE;
        state->current_screen = SCREEN_END;
    }
}

static void DrawCluePost(struct Game * game, BottomScreen * bottom_screen, struct InputState * input_state, int height) {
    int y = GetScreenHeight() - height + space;
    const char * c1 = "Room: ";
    DrawText(c1, space, y, 20, WHITE);
    DrawText(game->okRoom ? "OK" : "WRONG", space + MeasureText(c1, 20), y, 20, game->okRoom ? GREEN : RED);

    const char * c2 = "Item: ";
    DrawText(c2, space, y + 20 + space, 20, WHITE);
    DrawText(game->okItem ? "OK" : "WRONG", space + MeasureText(c2, 20), y + 20 + space, 20, game->okItem ? GREEN : RED);

    const char * c3 = "Murderer: ";
    DrawText(c3, space, y + 40 + space * 2, 20, WHITE);
    DrawText(game->okChar ? "OK" : "WRONG", space + MeasureText(c3, 20), y + 40 + space * 2, 20, game->okChar ? GREEN : RED);
    
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
            DrawIdle(state->text_input, screen, height);
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
            DrawCluePost(state->game, screen, state->text_input, height);
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
