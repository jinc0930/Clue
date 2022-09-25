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

typedef enum GameScreen { SCREEN_START = 0, SCREEN_GAMEPLAY, SCREEN_CHEATSHEET, SCREEN_END } GameScreen;
typedef enum BottomScreen { BSCREEN_IDLE, BSCREEN_TALK, BSCREEN_TAKE, BSCREEN_DROP, BSCREEN_CLUE, BSCREEN_CLUE_POST, BSCREEN_ERROR } BottomScreen;
typedef enum Lock { LOCK_KEYS = 1 << 0, LOCK_MOVE = 1 << 1 } Lock;

typedef struct ChatState {
    struct Character * talking_to;
    int count;
    int page;
} ChatState;


typedef struct FullState {
    struct Game *game;
    Texture2D characters_textures[N_CHARACTERS + 1];
    bool cheatsheet_items[N_ITEMS];
    bool cheatsheet_characters[N_CHARACTERS];
    bool cheatsheet_rooms[N_ROOMS];
    Vector2 mouse_pos;
    GameScreen current_screen;
    BottomScreen bottom_screen;
    struct ChatState chat;
    char text_input[MAX_INPUT_CHARS + 1];
    int frames_counter;
    Lock lock;
    char error[80];
    char player_name[MAX_INPUT_CHARS + 1];
} FullState;

// Draw map
void DrawMap(struct Room* map[9], Texture2D char_textures[], int _x, int _y, int width, int height);

// Draw instructions
void DrawSide(struct Game * game, int x, int y) ;

/*
 * SCREENS
 */

// Draw Cheatsheet
void DrawCheatSheet(struct FullState * state);

// Draw Bottom Screen
void DrawBottomScreen(struct FullState * state, int height);

// Draw Start
void DrawStart(char * text_input, int frames_count);

// Draw End
void DrawEnd(struct FullState * state);
#endif