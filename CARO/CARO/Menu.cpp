#include "Menu.h"
#include "Game.h" 
#include "raylib.h"
int g_menuChoice = 0; //
const char* MENU_ITEMS[] = { "NEW GAME", "LOAD GAME", "ABOUT", "SETTING", "EXIT" };
const char* MEMBERS[][3] = { 
    {"TUAN HUNG", "SOK MINH", "TRUONG GIANG"},
    {"GIA BAO", "QUOC ANH", "DUC THINH"} 
};
const char* TOOLS[] = { "C++", "RAYLIB" };
const int MENU_COUNT = 5;
const int MEMBERS_ROW = 2;
const int MEMBERS_COL = 3;
const int TOOLS_COUNT = 2;
std::map<std::string, Texture2D> game_textures; 


void InitMenu() {
    g_menuChoice = 0;
    game_textures["background"] = LoadTexture("assects/images/intro_bg.png");
    game_textures["cerydra"] = LoadTexture("assects/images/cerydra.png");
    game_textures["silverwolf"] = LoadTexture("assects/images/silverwolf_intro.png");
    game_textures["X_icon"] = LoadTexture("assects/images/X_intro.png");
    game_textures["O_icon"] = LoadTexture("assects/images/O_intro.png");
    game_textures["choosen_arrow"] = LoadTexture("assects/images/arrow_menu.png");
    game_textures["about_bg"] = LoadTexture("assects/images/setting_bg.png");
    SetTextureFilter(game_textures["background"], TEXTURE_FILTER_POINT);
}

void UpdateMenu(GameScreen& currentScreen) {
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        g_menuChoice = (g_menuChoice == 0) ? MENU_COUNT - 1 : g_menuChoice - 1;
    }
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        g_menuChoice = (g_menuChoice == MENU_COUNT - 1) ? 0 : g_menuChoice + 1;
    }

    if (IsKeyPressed(KEY_ENTER)) { //
        switch (g_menuChoice) {
        case 0: // NEW GAME
            InitGame(); 
            currentScreen = GAMEPLAY;
            break;
        case 1: // LOAD GAME
            loadGame("save.txt"); 
            currentScreen = GAMEPLAY;
            break;
        case 2: // ABOUT
            currentScreen = ABOUT;
            break;
        case 3: // SETTING
            currentScreen = SETTING;
            break;
        case 4: //EXIT
            break;
        }
    }
}

void DrawMenu() {
    ClearBackground(WHITE);

    Texture2D bg = game_textures["background"];
    Rectangle RectSource = { 0.0, 0.0, (float)bg.width, (float)bg.height };
    Rectangle RectDest = { 0.0, 0.0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    DrawTexturePro(bg, RectSource, RectDest, Vector2{ 0,0 }, 0, WHITE);

    int titleWidth = MeasureText("CARO GAME", 180);
    int title_posX = (SCREEN_WIDTH - titleWidth) / 2;
    int title_posY = 50;
    DrawTextAndBorder("CARO GAME", title_posX, title_posY, 180, 15, RED, SKYBLUE);
    DrawImage();

    int menu_start_y = 350; 
    int menu_spacing = 100;  
    for (int i = 0; i < MENU_COUNT; i++) {
        Color TColor = (i == g_menuChoice) ? YELLOW : LIGHTGRAY;
        Color BColor = (i == g_menuChoice) ? DARKPURPLE : BLACK;
        int textWidth = MeasureText(MENU_ITEMS[i], 100);
        int text_x_position = (SCREEN_WIDTH - textWidth) / 2;
        int text_y_position = menu_start_y + i * menu_spacing;
        int text_xend_position = (text_x_position + textWidth);
        DrawTextAndBorder(MENU_ITEMS[i], text_x_position, text_y_position, 100, 10, BColor, TColor);
        if (i == g_menuChoice) {
            Vector2 LeftArrow_position = { text_x_position - 50, text_y_position - 20 };
            Vector2 RightArrow_position = { text_xend_position + 50, text_y_position + 105 };
            DrawMenuArrow(LeftArrow_position, RightArrow_position);
        }
    }

}
void DrawMenuArrow(Vector2 LeftArrow, Vector2 RightArrow) {
    DrawTextureEx(game_textures["choosen_arrow"], LeftArrow, 90, 0.25, WHITE);
    DrawTextureEx(game_textures["choosen_arrow"], RightArrow, -90, 0.25, WHITE);
}

void DrawImage() {
    Vector2 cerydra_position = { 100, 600 };
    Vector2 silverwolf_position = { SCREEN_WIDTH - 1050, 600 };
    Vector2 O_position = { 120, 0 };
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

//Màn hình About
void UpdateAbout(GameScreen& currentScreen) {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
    }
}
void DrawAbout() {
    ClearBackground(WHITE);
    Texture2D bg = game_textures["about_bg"];
    Rectangle bgSource = { 0.0, 0.0, (float)bg.width, (float)bg.height };
    Rectangle bgDest = { 0.0, 0.0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    DrawTexturePro(bg, bgSource, bgDest, { 0, 0 }, 0.0, WHITE);
    float panelWidth = 1200;
    float panelHeight = 800;
    float panelX = (SCREEN_WIDTH - panelWidth) / 2;
    float panelY = (SCREEN_HEIGHT - panelHeight) / 2;
    float centerX = SCREEN_WIDTH / 2;
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(CLITERAL(Color) { 10, 20, 40, 255 }, 0.9f));
    DrawRectangleLinesEx(Rectangle{ panelX, panelY, panelWidth, panelHeight }, 5.0f, SKYBLUE);
    DrawRectangleLinesEx(Rectangle{ panelX + 10, panelY + 10, panelWidth - 20, panelHeight - 20 }, 2.0f, DARKBLUE);
    const char* mainTitle = "CARO GAME";
    int mainTitleSize = 100; 
    DrawText(mainTitle, centerX - MeasureText(mainTitle, mainTitleSize) / 2 + 6, panelY + 60, mainTitleSize, BLACK);
    DrawText(mainTitle, centerX - MeasureText(mainTitle, mainTitleSize) / 2, panelY + 50, mainTitleSize, YELLOW);
    int currentY = panelY + 180; 
    const char* teamTitle = "DEVELOPED BY [TEAM 2]";
    int headerSize = 40; 
    int nameSize = 32;   
    DrawText(teamTitle, centerX - MeasureText(teamTitle, headerSize) / 2, currentY, headerSize, SKYBLUE);
    currentY += 60; 
    int colSpacing = 380;
    for (int i = 0; i < MEMBERS_ROW; i++) {
        const char* name1 = MEMBERS[i][0];
        const char* name2 = MEMBERS[i][1];
        const char* name3 = MEMBERS[i][2];
        DrawText(name1, centerX - colSpacing - MeasureText(name1, nameSize) / 2, currentY, nameSize, WHITE);
        DrawText(name2, centerX - MeasureText(name2, nameSize) / 2, currentY, nameSize, WHITE);
        DrawText(name3, centerX + colSpacing - MeasureText(name3, nameSize) / 2, currentY, nameSize, WHITE);
        currentY += 50; 
    }
    currentY += 50; 
    const char* instrTitle = "INSTRUCTOR";
    const char* instrName = "TRUONG TOAN THINH";
    DrawText(instrTitle, centerX - MeasureText(instrTitle, headerSize) / 2, currentY, headerSize, SKYBLUE);
    currentY += 60;
    int instrNameSize = 55;
    DrawText(instrName, centerX - MeasureText(instrName, instrNameSize) / 2, currentY, instrNameSize, GOLD);
    currentY += 100; 
    const char* techTitle = "TECHNOLOGY USED";
    DrawText(techTitle, centerX - MeasureText(techTitle, headerSize) / 2, currentY, headerSize, SKYBLUE);
    currentY += 60;
    int techSize = 45;
    int techSpacing = 180; 
    DrawText(TOOLS[0], centerX - techSpacing - MeasureText(TOOLS[0], techSize) / 2, currentY, techSize, WHITE);
    DrawText("|", centerX - MeasureText("|", techSize) / 2, currentY, techSize, DARKGRAY);
    DrawText(TOOLS[1], centerX + techSpacing - MeasureText(TOOLS[1], techSize) / 2, currentY, techSize, WHITE);
    const char* footer = "Press [ESC] to Return Menu";
    int footerSize = 30;
    DrawText(footer, centerX - MeasureText(footer, footerSize) / 2, panelY + panelHeight - 50, footerSize, GRAY);
}
void UnloadAllTextures() {
    for (auto pair : game_textures) {
        UnloadTexture(pair.second);
    }
}