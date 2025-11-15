#include "View.h"
#include "Game.h" // View cần đọc dữ liệu từ Game

// Biến static để lưu trữ "sprite"
static Texture2D texX;
static Texture2D texO;

// GenXTexture (Nét "phình ra")
Texture2D GenXTexture(int size, Color color) {
    Image img = GenImageColor(size, size, BLANK);

    // Độ dày nét (vẽ 2-3 pixel lân cận)
    int thickness = 2;

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

// GenOTexture (Nét "phình ra")
Texture2D GenOTexture(int size, Color color) {
    Image img = GenImageColor(size, size, BLANK);

    int padding = 4; // Giữ lề
    int thickness = 4; // Tăng độ dày nét

    for (int t = 0; t < thickness; t++) {
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

void InitGameView() {
    // Dùng màu X/O đậm, dễ nhìn
    texX = GenXTexture(SPRITE_SIZE, CLITERAL(Color){ 255, 50, 50, 255 }); // Đỏ tươi
    texO = GenOTexture(SPRITE_SIZE, CLITERAL(Color){ 50, 100, 255, 255 }); // Xanh đậm
}

void UnloadGameView() {
    UnloadTexture(texX);
    UnloadTexture(texO);
}

// HÀM VẼ HOÀN CHỈNH
void DrawGameView() {
    // 1. Vẽ nền (LIGHTGRAY)
    ClearBackground(HSR_BG);

    // 2. Vẽ viền bàn cờ 2 lớp (theo yêu cầu thiết kế)
    // Viền chính bên ngoài
    DrawRectangle(BOARD_OFFSET_X - 15, BOARD_OFFSET_Y - 15, BOARD_WIDTH + 30, BOARD_WIDTH + 30, HSR_FRAME);
    // Viền phụ bên trong
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

    // 4. Vẽ quân cờ (Với SPRITE_SIZE mới)
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

    // 5. Vẽ thông tin 2 bên (Đã làm to, đổ màu nền, làm rõ nét)

    // --- Tính toán vị trí (Làm to ra) ---
    int leftPanelX = 100;
    int panelWidth = 450;   // Đã tăng
    int panelHeight = 350;  // Đã tăng
    int rightPanelX = BOARD_OFFSET_X + BOARD_WIDTH + 100;
    int panelY = BOARD_OFFSET_Y;

    // --- Xác định màu sắc (Rõ nét) ---
    Color p1Color = g_turn ? HSR_SQUARE_LIGHT : GRAY; // Active: Sáng | Inactive: Xám
    Color p2Color = !g_turn ? HSR_SQUARE_LIGHT : GRAY; // Active: Sáng | Inactive: Xám
    Color p1Highlight = g_turn ? YELLOW : HSR_FRAME; // Highlight vàng
    Color p2Highlight = !g_turn ? YELLOW : HSR_FRAME; // Highlight vàng

    // --- Vẽ Panel Player 1 (Bên Trái) ---
    DrawRectangle(leftPanelX, panelY, panelWidth, panelHeight, HSR_FRAME); // Đổ màu nền
    DrawRectangleLinesEx(Rectangle { (float)leftPanelX, (float)panelY, (float)panelWidth, (float)panelHeight }, 5.0f, p1Highlight); // Viền

    const char* p1Text = "PLAYER 1 [X]";
    int p1TextWidth = MeasureText(p1Text, 40);
    DrawText(p1Text, leftPanelX + (panelWidth - p1TextWidth) / 2, panelY + 60, 40, p1Color); // Vẽ chữ

    // --- Vẽ Panel Player 2 (Bên Phải) ---
    DrawRectangle(rightPanelX, panelY, panelWidth, panelHeight, HSR_FRAME); // Đổ màu nền
    DrawRectangleLinesEx(Rectangle { (float)rightPanelX, (float)panelY, (float)panelWidth, (float)panelHeight }, 5.0f, p2Highlight); // Viền

    const char* p2Text = "PLAYER 2 [O]";
    int p2TextWidth = MeasureText(p2Text, 40);
    DrawText(p2Text, rightPanelX + (panelWidth - p2TextWidth) / 2, panelY + 60, 40, p2Color); // Vẽ chữ

    // --- Vẽ thông tin phụ (Dùng màu BLACK để rõ trên nền LIGHTGRAY) ---
    DrawText("Nhấn [L] để Save", leftPanelX + 20, panelY + panelHeight + 20, 20, BLACK);
    DrawText("Nhấn [ESC] để về Menu", rightPanelX + 20, panelY + panelHeight + 20, 20, BLACK);

    // 6. Vẽ thông báo thắng (Giữ nguyên)
    if (g_status != PLAYING) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), CLITERAL(Color){ 0, 0, 0, 150 });

        const char* winnerText = "";
        if (g_status == X_WIN) winnerText = "PLAYER 1 WINS!";
        else if (g_status == O_WIN) winnerText = "PLAYER 2 WINS!";
        else if (g_status == DRAW) winnerText = "IT'S A DRAW!";

        int textWidth = MeasureText(winnerText, 40);
        DrawText(winnerText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 - 40, 40, GOLD);

        const char* restartText = "PRESS [Y] TO PLAY AGAIN";
        textWidth = MeasureText(restartText, 20);
        DrawText(restartText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2 + 20, 20, WHITE);
    }
}