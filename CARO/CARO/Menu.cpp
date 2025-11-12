#include "Menu.h"
#include "Game.h" // Menu cần gọi loadGame
#include "raylib.h"

int g_menuChoice = 0; //
const char* MENU_ITEMS[] = { "NEW GAME", "LOAD GAME", "ABOUT", "SETTING", "EXIT" };
const int MENU_COUNT = 5; //
Texture2D cerydra_intro;

void InitMenu() {
    g_menuChoice = 0;
    cerydra_intro = LoadTexture("assects/images/cerydra.png");
}

// Hàm này cập nhật logic menu
void UpdateMenu(GameScreen& currentScreen) {
    // Input tương đương _getch()
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        g_menuChoice = (g_menuChoice == 0) ? MENU_COUNT - 1 : g_menuChoice - 1;
    }
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        g_menuChoice = (g_menuChoice == MENU_COUNT - 1) ? 0 : g_menuChoice + 1;
    }

    if (IsKeyPressed(KEY_ENTER)) { //
        switch (g_menuChoice) {
        case 0: // NEW GAME
            InitGame(); // Chuẩn bị game mới
            currentScreen = GAMEPLAY;
            break;
        case 1: // LOAD GAME
            loadGame("save.txt"); //
            currentScreen = GAMEPLAY;
            break;
        case 2: // ABOUT
            currentScreen = ABOUT;
            break;
        case 3: // SETTING
            currentScreen = SETTING;
            break;
        case 4: // EXIT
            // Sẽ được xử lý trong main.cpp (WindowShouldClose)
            // Hoặc chúng ta có thể set 1 flag
            // Để đơn giản, ta sẽ để main.cpp tự thoát
            // Nhưng nếu muốn, bạn có thể tạo 1 biến bool g_shouldExit
            break;
        }
    }
}

// Hàm này vẽ menu, thay thế DrawMenuScreen
void DrawMenu() {
    ClearBackground(WHITE);

    // Tương đương DrawString
    int titleWidth = MeasureText("CARO GAME", 220);
    DrawText("CARO GAME", (GetScreenWidth() - titleWidth) / 2, 100, 220, SKYBLUE);

    // Vẽ các lựa chọn  
    for (int i = 0; i < MENU_COUNT; i++) {
        Color color = (i == g_menuChoice) ? GOLD : GRAY; //
        int textWidth = MeasureText(MENU_ITEMS[i], 100);
        DrawText(MENU_ITEMS[i], (GetScreenWidth() - textWidth) / 2, 600 + i * 200, 100, color);
    }

    DrawImage();
}

void DrawImage() {
    BeginDrawing();

    DrawTexture(cerydra_intro, 100, 200, WHITE);

    EndDrawing();
}

// --- Màn hình About ---
void UpdateAbout(GameScreen& currentScreen) {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
    }
}
void DrawAbout() { //
    ClearBackground(WHITE);
    DrawText("CARO GAME - MERGED VERSION", 50, 100, 40, WHITE);
    DrawText("Developed by [TEAM 2] & Merged by AI", 50, 160, 20, GRAY);
    DrawText("Press [ESC] or [ENTER] to return...", 50, 300, 20, SKYBLUE);
}

// --- Màn hình Setting ---
void UpdateSetting(GameScreen& currentScreen) {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
    }
}
void DrawSetting() { //
    ClearBackground(WHITE);
    DrawText("This feature is developing!!", 50, 100, 40, WHITE);
    DrawText("Press [ESC] or [ENTER] to return...", 50, 300, 20, SKYBLUE);
}