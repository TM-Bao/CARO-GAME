#include "raylib.h"
#include "Global.h"
#include "Menu.h"
#include "Game.h"
#include "View.h"
#include "settings.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CARO GAME"); //
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    GameScreen currentScreen = MENU;


    InitMenu();
    InitGameView();
    InitSetting();

    while (!WindowShouldClose()) {
        switch (currentScreen) {
        case MENU:
            UpdateMenu(currentScreen);
            break;
        case GAMEPLAY:
            UpdateGame(currentScreen);
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = MENU;
            }
            break;
        case ABOUT:
            UpdateAbout(currentScreen);
            break;
        case SETTING:
            UpdateSetting(currentScreen);
            break;
        }
        if (currentScreen == MENU && g_menuChoice == 4 && IsKeyPressed(KEY_ENTER)) {
            break;
        }

        BeginDrawing();

        switch (currentScreen) {
        case MENU:
            DrawMenu();
            break;
        case GAMEPLAY:
            DrawGameView();
            break;
        case ABOUT:
            DrawAbout();
            break;
        case SETTING:
            DrawSetting();
            break;
        }

        EndDrawing();
    }
//Dọn dẹp
    UnloadGameView();
    UnloadAllTextures();
    UnloadSetting();
    CloseWindow();

    return 0;
}