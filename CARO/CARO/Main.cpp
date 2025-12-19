#include "raylib.h"
#include "Global.h"
#include "Menu.h"
#include "Game.h"
#include "View.h"
#include "settings.h"
#include "Audio.h"
int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CARO GAME"); 
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    GameScreen currentScreen = MENU;


    InitMenu();
	InitAudioDevice();
	InitAudio();
    InitGameView();
    InitSetting();
    extern Music bgMusic;
    float startVolume = ((float)g_settings.musicVolume / 100.0f) * 0.3f;
    SetMusicVolume(bgMusic, startVolume);
    while (!WindowShouldClose()) {
        UpdateMusicStream(bgMusic);
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
	UnloadAudio();
    return 0;
}