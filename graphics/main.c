#include "raylib.h"
#include "../game.h"
#include "draw.h"
#include <time.h>
#include <stdbool.h>

// #define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 800;
const int screenHeight = 450;
const int panelWidth = 260;
const int chatHeight = 120;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void* arg_);     // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    struct Game game = makeGame();
    initGame(&game, "ME");
    FullState state = {
        .game = &game,
        .characters_textures = {
            LoadTexture("./graphics/resources/char_02.png"),
            LoadTexture("./graphics/resources/char_23.png"),
        },
        .cheatsheet_items = { false },
        .mouse_pos = (Vector2){ 0.0f, 0.0f },
        .current_screen = (GameScreen)SCREEN_GAMEPLAY,
        .chat = (ChatState){ NULL, 0, 0 },
        .bottom_screen = (BottomScreen)BSCREEN_IDLE,
        .input_state = (InputState){0, { 0 }},
        .frames_counter = 0,
        .lock = 0,
        .error = { 0 }
    };

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(UpdateDrawFrame, &state, 30, 1);
#else
    SetTargetFPS(30);   // Set our game to run at 30 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame(&state);
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    freeGame(&game);
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
static void resetOnMove(struct FullState * state) {
    state->bottom_screen = BSCREEN_IDLE;
    state->chat.talking_to = NULL;
}


void UpdateDrawFrame(void* arg_)
{
    // Update
    //----------------------------------------------------------------------------------
    struct FullState* state = arg_;
    struct Game * game = state->game;
    struct Room * current_room = game->avatar->location;
    struct ChatState * chat = &state->chat;

    state->frames_counter += 1; // note: if the game is running for more than 2 years at 30 fps, this causes a crash
    if (state->chat.talking_to != NULL) state->lock |= LOCK_MOVE;
    if (state->bottom_screen == BSCREEN_CLUE) state->lock |= LOCK_MOVE | LOCK_KEYS;
    state->mouse_pos = GetMousePosition();
    if ((state->lock & LOCK_KEYS) == 0 && IsKeyPressed(KEY_C)) state->current_screen = state->current_screen == SCREEN_CHEATSHEET ? SCREEN_GAMEPLAY : SCREEN_CHEATSHEET;
    if ((state->lock & LOCK_MOVE) == 0) {
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) (move(game, East), resetOnMove(state));
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) (move(game, West), resetOnMove(state));
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) (move(game, North), resetOnMove(state));
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) (move(game, South), resetOnMove(state));
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(state->current_screen) {
            case SCREEN_GAMEPLAY: {
                DrawMap(state->game->map, state->characters_textures, 8, 8, GetScreenWidth() - panelWidth, GetScreenHeight() - 20 - chatHeight);
                DrawBottomScreen(state, chatHeight);
                DrawSide(state->game, GetScreenWidth() - panelWidth + 8 * 3, GetScreenHeight() - 100 - chatHeight);
            } break;
            case SCREEN_CHEATSHEET: {
                DrawCheatSheet(state->cheatsheet_items, state->mouse_pos);
            } break;
        }

    EndDrawing();
    //----------------------------------------------------------------------------------
}