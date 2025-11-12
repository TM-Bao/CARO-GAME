#include "raylib.h"
#include "Global.h"
#include "Menu.h"
#include "Game.h"
#include "View.h"

int main(void) {
    // --- Khởi tạo (từ Main.cpp cũ) ---
    int display = GetCurrentMonitor();
    const int screenWidth = GetMonitorWidth(display);
    const int screenHeight = GetMonitorHeight(display);
    InitWindow(screenWidth, screenHeight, "ĐỒ ÁN CARO - Raylib 5.0"); //
    ToggleFullscreen();
    SetTargetFPS(60);

    // --- Khởi tạo các module ---
    GameScreen currentScreen = MENU;
    InitMenu();
    InitGameView();
    // InitGame() sẽ được gọi khi nhấn "New Game"

    // --- Vòng lặp chính (thay thế while(true)) ---
    while (!WindowShouldClose()) {

        //-----------------------------------------------------
        // UPDATE (Xử lý Logic)
        //-----------------------------------------------------
        switch (currentScreen) {
        case MENU:
            UpdateMenu(currentScreen);
            break;
        case GAMEPLAY:
            UpdateGame();
            // Cho phép quay về Menu
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

        // Thoát game nếu ở menu và chọn EXIT
        if (currentScreen == MENU && g_menuChoice == 4 && IsKeyPressed(KEY_ENTER)) {
            break; // Thoát vòng lặp while
        }

        //-----------------------------------------------------
        // DRAW (Xử lý Đồ họa)
        //-----------------------------------------------------
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

    // --- Dọn dẹp ---
    UnloadGameView(); // Giải phóng textures
    UnloadTexture(cerydra_intro);
    CloseWindow();

    return 0;
}