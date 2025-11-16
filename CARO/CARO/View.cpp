#include "View.h"
#include "Game.h" // View cần đọc dữ liệu từ Game

// === THÊM MỚI: Chỉ thị compiler (Từ file 2) ===
#pragma execution_character_set("utf-8")

// Biến static để lưu trữ "sprite"
static Texture2D texX;
static Texture2D texO;

// === THÊM MỚI: Biến lưu font (Từ file 2) ===
static Font customFont;

// === GIỮ NGUYÊN: GenXTexture (Nét "phình ra" từ file 1) ===
Texture2D GenXTexture(int size, Color color) {
    Image img = GenImageColor(size, size, BLANK);

    int thickness = 2; // Độ dày nét

    for (int i = 0; i < size; i++) {
        for (int t = -thickness; t <= thickness; t++) {
            // Đường chéo chính
            ImageDrawPixel(&img, i, i + t, color);
            ImageDrawPixel(&img, i + t, i, color);

            // Đường chéo phụ
            ImageDrawPixel(&img, i, (size - 1) - i + t, color);
            ImageDrawPixel(&img, i + t, (size - 1) - i, color);
        }
    }

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

// === GIỮ NGUYÊN: GenOTexture (Nét "phình ra" từ file 1) ===
Texture2D GenOTexture(int size, Color color) {
    Image img = GenImageColor(size, size, BLANK);

    int padding = 4; // Giữ nguyên lề
    int thickness = 4; // Độ dày nét mới

    for (int t = 0; t < thickness; t++) {
        // Vẽ 4 cạnh
        for (int i = padding; i < size - padding; i++) {
            ImageDrawPixel(&img, i, padding + t, color);
            ImageDrawPixel(&img, i, (size - 1) - padding - t, color);
            ImageDrawPixel(&img, padding + t, i, color);
            ImageDrawPixel(&img, (size - 1) - padding - t, i, color);
        }
    }

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

// === KẾT HỢP: InitGameView (Màu từ file 1 + Font từ file 2) ===
void InitGameView() {
    // Giữ màu sắc từ file 1
    texX = GenXTexture(SPRITE_SIZE, CLITERAL(Color){ 255, 50, 50, 255 }); // Đỏ tươi
    texO = GenOTexture(SPRITE_SIZE, CLITERAL(Color){ 50, 100, 255, 255 }); // Xanh đậm

    // Thêm code load font từ file 2
    // (Hãy chắc chắn bạn có file "resources/arial.ttf")
    customFont = LoadFontEx("resources/arial.ttf", 64, 0, 8000);
}

// === KẾT HỢP: UnloadGameView (Thêm UnloadFont) ===
void UnloadGameView() {
    UnloadTexture(texX);
    UnloadTexture(texO);
    // Thêm code unload font từ file 2
    UnloadFont(customFont);
}

// === HÀM VẼ HOÀN CHỈNH (Layout file 1 + DrawTextEx file 2) ===
void DrawGameView() {
    // 1. Vẽ nền (Từ file 1)
    ClearBackground(HSR_BG);

    // 2. Vẽ viền bàn cờ 2 lớp (Từ file 1)
    DrawRectangle(BOARD_OFFSET_X - 15, BOARD_OFFSET_Y - 15, BOARD_WIDTH + 30, BOARD_WIDTH + 30, HSR_FRAME);
    DrawRectangle(BOARD_OFFSET_X - 5, BOARD_OFFSET_Y - 5, BOARD_WIDTH + 10, BOARD_WIDTH + 10, HSR_SUB_FRAME);

    // 3. Vẽ bàn cờ checkerboard (Từ file 1)
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

    // 4. Vẽ quân cờ (Từ file 1)
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

    // 5. Vẽ thông tin 2 bên (Layout file 1 + code font file 2)

    // --- Tính toán vị trí (Từ file 1) ---
    int leftPanelX = 100;
    int panelWidth = 450;
    int panelHeight = 350;
    int rightPanelX = BOARD_OFFSET_X + BOARD_WIDTH + 100;
    int panelY = BOARD_OFFSET_Y;

    // --- Xác định màu sắc (Từ file 1 - dùng RED) ---
    Color p1Color = g_turn ? HSR_SQUARE_LIGHT : RED;
    Color p2Color = !g_turn ? HSR_SQUARE_LIGHT : RED;
    Color p1Highlight = g_turn ? YELLOW : HSR_FRAME;
    Color p2Highlight = !g_turn ? YELLOW : HSR_FRAME;

    // --- Vẽ Panel Player 1 (Bên Trái) ---
    DrawRectangle(leftPanelX, panelY, panelWidth, panelHeight, HSR_FRAME); // Đổ màu nền
    DrawRectangleLinesEx(Rectangle { (float)leftPanelX, (float)panelY, (float)panelWidth, (float)panelHeight }, 5.0f, p1Highlight); // Viền

    // === SỬA FONT (Từ file 2) ===
    const char* p1Text = "PLAYER 1 [X]";
    Vector2 p1TextSize = MeasureTextEx(customFont, p1Text, 40.0f, 1.0f);
    DrawTextEx(customFont, p1Text,
        Vector2{ (float)(leftPanelX + (panelWidth - p1TextSize.x) / 2), (float)(panelY + 60) },
        40.0f, 1.0f, p1Color);

    // --- Vẽ Panel Player 2 (Bên Phải) ---
    DrawRectangle(rightPanelX, panelY, panelWidth, panelHeight, HSR_FRAME); // Đổ màu nền
    DrawRectangleLinesEx(Rectangle { (float)rightPanelX, (float)panelY, (float)panelWidth, (float)panelHeight }, 5.0f, p2Highlight); // Viền

    // === SỬA FONT (Từ file 2) ===
    const char* p2Text = "PLAYER 2 [O]";
    Vector2 p2TextSize = MeasureTextEx(customFont, p2Text, 40.0f, 1.0f);
    DrawTextEx(customFont, p2Text,
        Vector2{ (float)(rightPanelX + (panelWidth - p2TextSize.x) / 2), (float)(panelY + 60) },
        40.0f, 1.0f, p2Color);

    // --- Vẽ thông tin phụ (Lưu, Thoát) ---
    // === SỬA FONT (Từ file 2) ===
    DrawTextEx(customFont, u8"Nhấn [L] để Save",
        Vector2{ (float)(leftPanelX + 20), (float)(panelY + panelHeight + 20) },
        20.0f, 1.0f, BLACK);

    DrawTextEx(customFont, u8"Nhấn [ESC] để về Menu",
        Vector2{ (float)(rightPanelX + 20), (float)(panelY + panelHeight + 20) },
        20.0f, 1.0f, BLACK);

    // 6. Vẽ thông báo thắng
    if (g_status != PLAYING) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), CLITERAL(Color){ 0, 0, 0, 150 });

        const char* winnerText = "";
        if (g_status == X_WIN) winnerText = "PLAYER 1 WINS!";
        else if (g_status == O_WIN) winnerText = "PLAYER 2 WINS!";
        else if (g_status == DRAW) winnerText = "IT'S A DRAW!";

        // === SỬA FONT (Từ file 2) ===
        Vector2 textSize = MeasureTextEx(customFont, winnerText, 40.0f, 1.0f);
        DrawTextEx(customFont, winnerText,
            Vector2{ (GetScreenWidth() - textSize.x) / 2, (float)(GetScreenHeight() / 2 - 40) },
            40.0f, 1.0f, GOLD);

        const char* restartText = "PRESS [Y] TO PLAY AGAIN";

        // === SỬA FONT (Từ file 2) ===
        textSize = MeasureTextEx(customFont, restartText, 20.0f, 1.0f);
        DrawTextEx(customFont, restartText,
            Vector2{ (GetScreenWidth() - textSize.x) / 2, (float)(GetScreenHeight() / 2 + 20) },
            20.0f, 1.0f, WHITE);
    }

    // === GIỮ NGUYÊN: highlight ô (Từ file 1) ===
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