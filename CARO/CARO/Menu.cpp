#include "Menu.h"
#include "Game.h" // Menu cần gọi loadGame
#include "raylib.h"

int g_menuChoice = 0; //
const char* MENU_ITEMS[] = { "NEW GAME", "LOAD GAME", "ABOUT", "SETTING", "EXIT" };
const int MENU_COUNT = 5; 
std::map<std::string, Texture2D> game_textures; // Map luu theo Key: Value;


void InitMenu() {
    g_menuChoice = 0;
    game_textures["background"] = LoadTexture("assects/images/intro_bg.png");
    game_textures["cerydra"] = LoadTexture("assects/images/cerydra.png");
    game_textures["silverwolf"] = LoadTexture("assects/images/silverwolf_intro.png");
    game_textures["X_icon"] = LoadTexture("assects/images/X_intro.png");
    game_textures["O_icon"] = LoadTexture("assects/images/O_intro.png");
    game_textures["choosen_arrow"] = LoadTexture("assects/images/arrow_menu.png");
    SetTextureFilter(game_textures["background"], TEXTURE_FILTER_POINT);
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

    Texture2D bg = game_textures["background"];
    Rectangle RectSource = { 0.0, 0.0, (float)bg.width, (float)bg.height };
    // ⭐️ SỬA LẠI: Dùng kích thước cố định
    Rectangle RectDest = { 0.0, 0.0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    DrawTexturePro(bg, RectSource, RectDest, Vector2{ 0,0 }, 0, WHITE);

    // Tương đương DrawString
    int titleWidth = MeasureText("CARO GAME", 180);
    // ⭐️ SỬA LẠI: Dùng kích thước cố định
    int title_posX = (SCREEN_WIDTH - titleWidth) / 2;
    int title_posY = 50;
    DrawTextAndBorder("CARO GAME", title_posX, title_posY, 180, 15, RED, SKYBLUE);

    DrawImage();

    int menu_start_y = 350; 
    int menu_spacing = 100;  

    // Vẽ các lựa chọn  
    for (int i = 0; i < MENU_COUNT; i++) {
        Color TColor = (i == g_menuChoice) ? YELLOW : LIGHTGRAY;
        Color BColor = (i == g_menuChoice) ? DARKPURPLE : BLACK;
        int textWidth = MeasureText(MENU_ITEMS[i], 100);
        // ⭐️ SỬA LẠI: Dùng kích thước cố định
        int text_x_position = (SCREEN_WIDTH - textWidth) / 2;
        // ⭐️ SỬA LẠI Y:
        int text_y_position = menu_start_y + i * menu_spacing;
        int text_xend_position = (text_x_position + textWidth);
        DrawTextAndBorder(MENU_ITEMS[i], text_x_position, text_y_position, 100, 10, BColor, TColor);
        if (i == g_menuChoice) {
            Vector2 LeftArrow_position = { text_x_position - 50, text_y_position - 20 };
            Vector2 RightArrow_position = { text_xend_position + 50, text_y_position + 105 };
            DrawMenuArrow(LeftArrow_position, RightArrow_position);
        }
    }

    // ⭐️ ĐÃ DI CHUYỂN LÊN TRÊN
    // DrawImage();
}
void DrawMenuArrow(Vector2 LeftArrow, Vector2 RightArrow) {
    DrawTextureEx(game_textures["choosen_arrow"], LeftArrow, 90, 0.25, WHITE);
    DrawTextureEx(game_textures["choosen_arrow"], RightArrow, -90, 0.25, WHITE);
}

void DrawImage() {

    Vector2 cerydra_position = { 100, 600 };
    // ⭐️ SỬA LẠI: Dùng kích thước cố định
    Vector2 silverwolf_position = { SCREEN_WIDTH - 1050, 600 };
    Vector2 O_position = { 120, 0 };
    // ⭐️ SỬA LẠI: Dùng kích thước cố định
    Vector2 X_position = { SCREEN_WIDTH - 380, 15 };

    DrawTextureEx(game_textures["O_icon"], O_position, 0, 0.5, WHITE);
    DrawTextureEx(game_textures["X_icon"], X_position, 0, 0.3, WHITE);
}

void DrawTextAndBorder(const char* text, int posX, int posY, int Size, int Outline, Color BColor, Color TColor) {
    DrawText(text, posX - Outline, posY, Size, BColor);
    DrawText(text, posX + Outline, posY, Size, BColor);
    DrawText(text, posX, posY + Outline, Size, BColor);
    DrawText(text, posX, posY - Outline, Size, BColor);
    DrawText(text, posX, posY, Size, TColor);
}

// --- Màn hình About ---
void UpdateAbout(GameScreen& currentScreen) {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
    }
}
void DrawAbout() { //
    ClearBackground(WHITE);
    DrawText("CARO GAME", 50, 100, 40, WHITE);
    DrawText("Developed by [TEAM 2]", 50, 160, 20, GRAY);
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

// Clear All Textures
void UnloadAllTextures() {
    for (auto pair : game_textures) {
        UnloadTexture(pair.second);
    }
}