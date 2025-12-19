#include "View.h"
#include "Game.h"
#include "settings.h"
#pragma execution_character_set("utf-8")

static Texture2D texX;
static Texture2D texO;
static Font customFont;


Texture2D GenXTexture(int size, Color color) {
    Image img = GenImageColor(size, size, BLANK);
    int thickness = 5; 
    for (int i = 0; i < size; i++) {
        for (int t = -thickness; t <= thickness; t++) {
            ImageDrawPixel(&img, i, i + t, color);
            ImageDrawPixel(&img, i + t, i, color);
            ImageDrawPixel(&img, i, (size - 1) - i + t, color);
            ImageDrawPixel(&img, i + t, (size - 1) - i, color);
        }
    }

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}


Texture2D GenOTexture(int size, Color color) {
    RenderTexture2D target = LoadRenderTexture(size, size);
    BeginTextureMode(target);
    ClearBackground(BLANK); 
    Vector2 center = { size / 2.0f, size / 2.0f };
    float outerRadius = (size / 2.0f) - 4.0f; 
    float innerRadius = outerRadius - 7.0f;   
    DrawRing(center, innerRadius, outerRadius, 0, 360, 0, color);
    EndTextureMode();
    Image img = LoadImageFromTexture(target.texture);
    ImageFlipVertical(&img); 
    Texture2D finalTex = LoadTextureFromImage(img);


    UnloadImage(img);
    UnloadRenderTexture(target);
    return finalTex;
}

void InitGameView() {
    // Load X và O
    texX = GenXTexture(SPRITE_SIZE, CLITERAL(Color){ 255, 50, 50, 255 }); // Đỏ tươi
    texO = GenOTexture(SPRITE_SIZE, CLITERAL(Color){ 50, 100, 255, 255 }); // Xanh đậm
    customFont = LoadFontEx("resources/arial.ttf", 64, 0, 8000);
}

void UnloadGameView() {
    UnloadTexture(texX);
    UnloadTexture(texO);
    UnloadFont(customFont);
}

void DrawGameView() {
    // 1. Vẽ nền 
    ClearBackground(HSR_BG);

    // 2. Vẽ viền bàn cờ 2 lớp 
    DrawRectangle(BOARD_OFFSET_X - 15, BOARD_OFFSET_Y - 15, BOARD_WIDTH + 30, BOARD_WIDTH + 30, HSR_FRAME);
    DrawRectangle(BOARD_OFFSET_X - 5, BOARD_OFFSET_Y - 5, BOARD_WIDTH + 10, BOARD_WIDTH + 10, HSR_SUB_FRAME);

    // 3. Vẽ bàn cờ checkerboard 
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            Color cellColor = ((x + y) % 2 == 0) ? HSR_SQUARE_LIGHT : HSR_SQUARE_DARK;
            DrawRectangle(
                BOARD_OFFSET_X + x * CELL_SIZE,
                BOARD_OFFSET_Y + y * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
                cellColor
            );
        }
    }

    // 4. Vẽ quân cờ 
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (g_board[y][x] != EMPTY) {
                float drawX = BOARD_OFFSET_X + x * CELL_SIZE + (CELL_SIZE - SPRITE_SIZE) / 2;
                float drawY = BOARD_OFFSET_Y + y * CELL_SIZE + (CELL_SIZE - SPRITE_SIZE) / 2;

                Texture2D texToDraw = (g_board[y][x] == X) ? texX : texO;
                DrawTexture(texToDraw, drawX, drawY, WHITE);
            }
        }
    }

    // 5. Vẽ thông tin 2 bên 
    int leftPanelX = 100;
    int panelWidth = 450;
    int panelHeight = 350;
    int rightPanelX = BOARD_OFFSET_X + BOARD_WIDTH + 100;
    int panelY = BOARD_OFFSET_Y;

    Color p1Color = g_turn ? HSR_SQUARE_LIGHT : RED;
    Color p2Color = !g_turn ? HSR_SQUARE_LIGHT : RED;
    Color p1Highlight = g_turn ? YELLOW : HSR_FRAME;
    Color p2Highlight = !g_turn ? YELLOW : HSR_FRAME;
//Player 1
    DrawRectangle(leftPanelX, panelY, panelWidth, panelHeight, HSR_FRAME);
    DrawRectangleLinesEx(Rectangle{ (float)leftPanelX, (float)panelY, (float)panelWidth, (float)panelHeight }, 5.0f, p1Highlight);

    const char* p1Text = "PLAYER 1 [X]";
    Vector2 p1TextSize = MeasureTextEx(customFont, p1Text, 40.0f, 1.0f);
    DrawTextEx(customFont, p1Text,
        Vector2{ (float)(leftPanelX + (panelWidth - p1TextSize.x) / 2), (float)(panelY + 60) },
        40.0f, 1.0f, p1Color);
    if (g_settings.inputMode == INPUT_KEYBOARD) {
        Color guideColor = g_turn ? GOLD : GRAY;

        DrawTextEx(customFont, "MOVE: [W] [A] [S] [D]",
            Vector2{ (float)(leftPanelX + 40), (float)(panelY + 120) }, 25.0f, 1.0f, guideColor);

        DrawTextEx(customFont, "PLACE: [SPACE] / [ENTER]",
            Vector2{ (float)(leftPanelX + 40), (float)(panelY + 160) }, 25.0f, 1.0f, guideColor);
    }
    else {
        DrawTextEx(customFont, "MODE: MOUSE CLICK",
            Vector2{ (float)(leftPanelX + 40), (float)(panelY + 140) }, 30.0f, 1.0f, LIGHTGRAY);
    }
//Player 2
    DrawRectangle(rightPanelX, panelY, panelWidth, panelHeight, HSR_FRAME);
    DrawRectangleLinesEx(Rectangle{ (float)rightPanelX, (float)panelY, (float)panelWidth, (float)panelHeight }, 5.0f, p2Highlight);

    const char* p2Text = "PLAYER 2 [O]";
    Vector2 p2TextSize = MeasureTextEx(customFont, p2Text, 40.0f, 1.0f);
    DrawTextEx(customFont, p2Text,
        Vector2{ (float)(rightPanelX + (panelWidth - p2TextSize.x) / 2), (float)(panelY + 60) },
        40.0f, 1.0f, p2Color);
    float subTextSize = 32.0f;
    if (g_settings.inputMode == INPUT_KEYBOARD) {
        Color guideColor = !g_turn ? GOLD : GRAY;

        DrawTextEx(customFont, "MOVE: [ARROW KEYS]",
            Vector2{ (float)(rightPanelX + 40), (float)(panelY + 120) }, 25.0f, 1.0f, guideColor);

        DrawTextEx(customFont, "PLACE: [SPACE] / [ENTER]",
            Vector2{ (float)(rightPanelX + 40), (float)(panelY + 160) }, 25.0f, 1.0f, guideColor);
    }
    else {
        DrawTextEx(customFont, "MODE: MOUSE CLICK",
            Vector2{ (float)(rightPanelX + 40), (float)(panelY + 140) }, 30.0f, 1.0f, LIGHTGRAY);
    }

    const char* saveText = u8"PRESS [L] TO SAVE";
    Vector2 saveTextSize = MeasureTextEx(customFont, saveText, subTextSize, 1.0f);
    float saveX = leftPanelX + (panelWidth - saveTextSize.x) / 2;
    DrawTextEx(customFont, saveText,
        Vector2{ saveX, (float)(panelY + panelHeight + 20) },
        subTextSize, 1.0f, BLACK);
    const char* menuText = u8"PRESS [ESC] TO RETURN TO MENU";
    Vector2 menuTextSize = MeasureTextEx(customFont, menuText, subTextSize, 1.0f);
    float menuX = rightPanelX + (panelWidth - menuTextSize.x) / 2;
    DrawTextEx(customFont, menuText,
        Vector2{ menuX, (float)(panelY + panelHeight + 20) },
        subTextSize, 1.0f, BLACK);

    if (g_status != PLAYING) {
        if (g_status == X_WIN || g_status == O_WIN) {
            Vector2 startPos = {
                (float)(BOARD_OFFSET_X + winStartX * CELL_SIZE + CELL_SIZE / 2),
                (float)(BOARD_OFFSET_Y + winStartY * CELL_SIZE + CELL_SIZE / 2)
            };
            Vector2 endPos = {
                (float)(BOARD_OFFSET_X + winEndX * CELL_SIZE + CELL_SIZE / 2),
                (float)(BOARD_OFFSET_Y + winEndY * CELL_SIZE + CELL_SIZE / 2)
            };

            DrawLineEx(startPos, endPos, 12.0f, WHITE);
            DrawLineEx(startPos, endPos, 6.0f, GOLD);
            DrawCircleV(startPos, 6.0f, WHITE);
            DrawCircleV(endPos, 6.0f, WHITE);
            DrawCircleV(startPos, 3.0f, GOLD);
            DrawCircleV(endPos, 3.0f, GOLD);
        }


        const char* winnerText = "";
        Color winnerColor = WHITE;

        if (g_status == X_WIN) {
            winnerText = "PLAYER 1 WINS!";
            winnerColor = RED;
        }
        else if (g_status == O_WIN) {
            winnerText = "PLAYER 2 WINS!";
            winnerColor = SKYBLUE;
        }
        else {
            winnerText = "DRAW GAME!";
            winnerColor = LIGHTGRAY;
        }
        float titleSize = 80.0f;
        float subSize = 30.0f;
        Vector2 winnerSize = MeasureTextEx(customFont, winnerText, titleSize, 1.0f);
        Vector2 textPos = { (GetScreenWidth() - winnerSize.x) / 2, 30.0f };
        float stroke = 3.0f;
        DrawTextEx(customFont, winnerText, Vector2{ textPos.x - stroke, textPos.y }, titleSize, 1.0f, BLACK);
        DrawTextEx(customFont, winnerText, Vector2{ textPos.x + stroke, textPos.y }, titleSize, 1.0f, BLACK);
        DrawTextEx(customFont, winnerText, Vector2{ textPos.x, textPos.y - stroke }, titleSize, 1.0f, BLACK);
        DrawTextEx(customFont, winnerText, Vector2{ textPos.x, textPos.y + stroke }, titleSize, 1.0f, BLACK);
        DrawTextEx(customFont, winnerText, textPos, titleSize, 1.0f, winnerColor);
        const char* guideText = "PRESS [Y] TO RESTART   |   [ESC] TO MENU";
        Vector2 guideSize = MeasureTextEx(customFont, guideText, subSize, 1.0f);
        Vector2 guidePos = { (GetScreenWidth() - guideSize.x) / 2, GetScreenHeight() - 60.0f };
        stroke = 2.0f;
        DrawTextEx(customFont, guideText, Vector2{ guidePos.x - stroke, guidePos.y }, subSize, 1.0f, BLACK);
        DrawTextEx(customFont, guideText, Vector2{ guidePos.x + stroke, guidePos.y }, subSize, 1.0f, BLACK);
        DrawTextEx(customFont, guideText, Vector2{ guidePos.x, guidePos.y - stroke }, subSize, 1.0f, BLACK);
        DrawTextEx(customFont, guideText, Vector2{ guidePos.x, guidePos.y + stroke }, subSize, 1.0f, BLACK);


        DrawTextEx(customFont, guideText, guidePos, subSize, 1.0f, WHITE);
        Rectangle btnSetting = { (float)SCREEN_WIDTH - 170, 20, 150, 50 };
        bool isHover = CheckCollisionPointRec(GetMousePosition(), btnSetting);
        Color btnColor = isHover ? SKYBLUE : RED;
        DrawRectangleRec(btnSetting, btnColor);
        DrawRectangleLinesEx(btnSetting, 3, DARKBLUE);
        DrawText("SETTINGS", btnSetting.x + 25, btnSetting.y + 15, 20, DARKBLUE);
    }
    for (int i = 0; i < 5; i++) {
        DrawRectangleLines(
            BOARD_OFFSET_X + g_cursorX * CELL_SIZE + i,
            BOARD_OFFSET_Y + g_cursorY * CELL_SIZE + i,
            CELL_SIZE - 2 * i,
            CELL_SIZE - 2 * i,
            YELLOW
        );
    }
}