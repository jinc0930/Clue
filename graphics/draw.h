#include "raylib.h"
#include "../game.h"
#include "../items.h"
#include "../character.h"
#include "../rooms.h"
#include <stdbool.h>

#ifndef DRAW_H
#define DRAW_H
#define MAX_INPUT_CHARS 16
#define MAX_ERROR_CHARS 64

typedef enum GameScreen { SCREEN_START = 0, SCREEN_GAMEPLAY, SCREEN_CHEATSHEET, SCREEN_ENDING } GameScreen;
typedef enum BottomScreen { BSCREEN_IDLE, BSCREEN_TALK, BSCREEN_TAKE, BSCREEN_DROP, BSCREEN_CLUE, BSCREEN_CLUE_POST, BSCREEN_ERROR } BottomScreen;
typedef enum Lock { LOCK_KEYS = 1 << 0, LOCK_MOVE = 1 << 1 } Lock;

typedef struct ChatState {
    struct Character * talking_to;
    int count;
    int page;
} ChatState;

typedef struct InputState {
    int count;
    char text[MAX_INPUT_CHARS + 1];
} InputState;

typedef struct FullState {
    struct Game *game;
    Texture2D characters_textures[N_CHARACTERS];
    bool cheatsheet_items[N_ITEMS];
    Vector2 mouse_pos;
    GameScreen current_screen;
    BottomScreen bottom_screen;
    struct ChatState chat;
    struct InputState input_state;
    int frames_counter;
    Lock lock;
    char error[80];
} FullState;

// Draw map
void DrawMap(struct Room* map[9], Texture2D char_textures[], int _x, int _y, int width, int height);

// Draw instructions
void DrawSide(struct Game * game, int x, int y) ;

/*
 * SCREENS
 */

// Draw Cheatsheet
void DrawCheatSheet(bool items[N_ITEMS], Vector2 mousePoint) ;

// Draw Bottom Screen
void DrawBottomScreen(struct FullState * state, int height);

#endif