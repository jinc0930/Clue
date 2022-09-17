#include "raylib.h"
#include "../game.h"
#include "draw.h"
#include <time.h>
// #define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 800;
const int screenHeight = 450;
struct FullState {
    struct Game *game;
    Texture2D characters_textures[9];
};

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
    
    struct FullState state = {
        .game = &game,
        .characters_textures = {
            LoadTexture("./graphics/resources/char_02.png")
        }
    };

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(UpdateDrawFrame, &state, 60, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
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
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void* arg_)
{
    struct FullState* state = arg_;
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawMap(state->game->map, state->characters_textures, 8, 8, GetScreenWidth() - 260, GetScreenHeight() - 16);
        DrawItems(state->game->avatar->location, GetScreenWidth() - 260 + 8 * 3, 8);

        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}