#include "View.h"
#include "Game.h" 
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
    // ==== KEYCAP CHO PLAYER 1 (W / A S D) ====
    float keySize = 50;
    float gap = 30;                // khoảng cách ngang A–S–D
    float verticalGap = 5;        // khoảng cách dọc W–S (mới thêm)

    float cx = leftPanelX + panelWidth / 2;    // tâm panel
    float topY = panelY + 150;                 // hàng W
    float bottomY = topY + keySize + verticalGap;  // hàng ASD

    // ===== W =====
    DrawRectangle(cx - keySize / 2, topY, keySize, keySize, WHITE);
    DrawRectangleLines(cx - keySize / 2, topY, keySize, keySize, BLACK);
    DrawText("W", cx - 10, topY + 10, 32, BLACK);

    // ===== A =====
    DrawRectangle(cx - keySize - gap, bottomY, keySize, keySize, WHITE);
    DrawRectangleLines(cx - keySize - gap, bottomY, keySize, keySize, BLACK);
    DrawText("A", cx - keySize - gap + keySize / 2 - 10, bottomY + 10, 32, BLACK);

    // ===== S =====
    DrawRectangle(cx - keySize / 2, bottomY, keySize, keySize, WHITE);
    DrawRectangleLines(cx - keySize / 2, bottomY, keySize, keySize, BLACK);
    DrawText("S", cx - 10, bottomY + 10, 32, BLACK);

    // ===== D =====
    DrawRectangle(cx + gap, bottomY, keySize, keySize, WHITE);
    DrawRectangleLines(cx + gap, bottomY, keySize, keySize, BLACK);
    DrawText("D", cx + gap + keySize / 2 - 10, bottomY + 10, 32, BLACK);

//Player 2
    DrawRectangle(rightPanelX, panelY, panelWidth, panelHeight, HSR_FRAME);
    DrawRectangleLinesEx(Rectangle{ (float)rightPanelX, (float)panelY, (float)panelWidth, (float)panelHeight }, 5.0f, p2Highlight);

    const char* p2Text = "PLAYER 2 [O]";
    Vector2 p2TextSize = MeasureTextEx(customFont, p2Text, 40.0f, 1.0f);
    DrawTextEx(customFont, p2Text,
        Vector2{ (float)(rightPanelX + (panelWidth - p2TextSize.x) / 2), (float)(panelY + 60) },
        40.0f, 1.0f, p2Color);
    // ==== KEYCAP CHO PLAYER 2 (ARROWS) ====
    int keySize2 = 50;
    int keySpacing2 = 30;          // khoảng cách ngang (LEFT–DOWN–RIGHT)
    int verticalSpacing2 = 5;     // khoảng cách dọc (UP–DOWN)

    int cx2 = rightPanelX + panelWidth / 2;   // tâm panel
    int upY = panelY + 150;                  // vị trí nút UP
    int downY = upY + keySize2 + verticalSpacing2;   // vị trí hàng LEFT–DOWN–RIGHT

    // ===== UP ( ^ ) =====
    Rectangle keyUp = { cx2 - keySize2 / 2, upY, keySize2, keySize2 };
    DrawRectangleRec(keyUp, WHITE);
    DrawRectangleLinesEx(keyUp, 2, BLACK);
    DrawText("^", keyUp.x + keySize2 / 2 - 6, keyUp.y + 6, 28, BLACK);

    // ===== LEFT ( < ) =====
    Rectangle keyLeft = { cx2 - keySize2 - keySpacing2, downY, keySize2, keySize2 };
    DrawRectangleRec(keyLeft, WHITE);
    DrawRectangleLinesEx(keyLeft, 2, BLACK);
    DrawText("<", keyLeft.x + keySize2 / 2 - 7, keyLeft.y + 6, 28, BLACK);

    // ===== DOWN ( v ) =====
    Rectangle keyDown = { cx2 - keySize2 / 2, downY, keySize2, keySize2 };
    DrawRectangleRec(keyDown, WHITE);
    DrawRectangleLinesEx(keyDown, 2, BLACK);
    DrawText("v", keyDown.x + keySize2 / 2 - 7, keyDown.y + 8, 28, BLACK);

    // ===== RIGHT ( > ) =====
    Rectangle keyRight = { cx2 + keySpacing2, downY, keySize2, keySize2 };
    DrawRectangleRec(keyRight, WHITE);
    DrawRectangleLinesEx(keyRight, 2, BLACK);
    DrawText(">", keyRight.x + keySize2 / 2 - 7, keyRight.y + 6, 28, BLACK);

    float subTextSize = 32.0f;

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

    // 6. Vẽ thông báo thắng
    if (g_status != PLAYING) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), CLITERAL(Color){ 0, 0, 0, 180 });

        const char* winnerText = "";
        if (g_status == X_WIN) winnerText = "PLAYER 1 WINS!";
        else if (g_status == O_WIN) winnerText = "PLAYER 2 WINS!";
        else if (g_status == DRAW) winnerText = "IT'S A DRAW!";
        float titleSize = 80.0f;
        Vector2 textSize = MeasureTextEx(customFont, winnerText, titleSize, 1.0f);
        Vector2 textPos = { (float)(GetScreenWidth() - textSize.x) / 2, (float)(GetScreenHeight() / 2 - 80) };
        DrawTextEx(customFont, winnerText, Vector2{ textPos.x + 5, textPos.y + 5 }, titleSize, 1.0f, BLACK);
        DrawTextEx(customFont, winnerText, textPos, titleSize, 1.0f, GOLD);
        const char* restartText = "PRESS [Y] TO PLAY AGAIN OR [ESC] TO RETURN TO MENU";
        float subSize = 30.0f;
        textSize = MeasureTextEx(customFont, restartText, subSize, 1.0f);
        textPos = Vector2{ (float)(GetScreenWidth() - textSize.x) / 2, (float)(GetScreenHeight() / 2 + 40) };
        DrawTextEx(customFont, restartText, Vector2{ textPos.x + 3, textPos.y + 3 }, subSize, 1.0f, BLACK);
        DrawTextEx(customFont, restartText, textPos, subSize, 1.0f, WHITE);
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