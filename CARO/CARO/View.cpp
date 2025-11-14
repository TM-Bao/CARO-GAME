#include <raylib.h>
#include "View.h"
#include "Game.h" // View cần đọc dữ liệu từ Game

// Biến static để lưu trữ "sprite"
static Texture2D texX;
static Texture2D texO;
static Font customFont;

// "Thủ thuật" này tương đương với file Font.cpp của bạn
// Nó tạo ra một Texture X bằng cách "vẽ" từng pixel
Texture2D GenXTexture(int size, Color color) {
    Image img = GenImageColor(size, size, BLANK);
    for (int i = 0; i < size; i++) {
        ImageDrawPixel(&img, i, i, color);
        ImageDrawPixel(&img, i, i + 1, color);
        ImageDrawPixel(&img, i + 1, i, color);
        ImageDrawPixel(&img, i, (size - 1) - i, color);
        ImageDrawPixel(&img, i, (size - 1) - i + 1, color);
        ImageDrawPixel(&img, i + 1, (size - 1) - i, color);
    }
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}
// "Thủ thuật" tạo Texture O (Phiên bản sửa, không dùng ImageDrawRing)
Texture2D GenOTexture(int size, Color color) {
    Image img = GenImageColor(size, size, BLANK);

    // Chúng ta sẽ tự vẽ một cái viền vuông 2 pixel
    // Giống hệt cách bạn vẽ X, nhưng là hình vuông
    int padding = 4; // Để lại 4 pixel lề
    int thickness = 2; // Độ dày 2 pixel

    for (int t = 0; t < thickness; t++) {
        // Vẽ 4 cạnh
        for (int i = padding; i < size - padding; i++) {
            // Cạnh trên
            ImageDrawPixel(&img, i, padding + t, color);
            // Cạnh dưới
            ImageDrawPixel(&img, i, (size - 1) - padding - t, color);
            // Cạnh trái
            ImageDrawPixel(&img, padding + t, i, color);
            // Cạnh phải
            ImageDrawPixel(&img, (size - 1) - padding - t, i, color);
        }
    }

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

void InitGameView() {
    texX = GenXTexture(SPRITE_SIZE, MAROON); // Tương đương màu 12 (đỏ)
    texO = GenOTexture(SPRITE_SIZE, BLUE);   // Tương đương màu 9 (xanh)
    customFont = LoadFontEx("resources/arial.ttf", 64, 0, 100000);
}

void UnloadGameView() {
    UnloadTexture(texX);
    UnloadTexture(texO);
    UnloadFont(customFont);
}

// Hàm này vẽ toàn bộ màn hình game
void DrawGameView() {
    // 1. Vẽ nền
    ClearBackground(GRASS_GREEN);

    // 2. Vẽ khung và nền bàn cờ (tương đương DrawChessBoard)
    DrawRectangle(BOARD_OFFSET_X - 10, BOARD_OFFSET_Y - 10, BOARD_WIDTH + 20, BOARD_WIDTH + 20, BOARD_FRAME);
    DrawRectangle(BOARD_OFFSET_X, BOARD_OFFSET_Y, BOARD_WIDTH, BOARD_WIDTH, BOARD_BG);

    // 3. Vẽ đường kẻ
    for (int i = 0; i <= BOARD_SIZE; i++) {
        DrawLine(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y, BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y + BOARD_WIDTH, BOARD_LINE);
        DrawLine(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * CELL_SIZE, BOARD_OFFSET_X + BOARD_WIDTH, BOARD_OFFSET_Y + i * CELL_SIZE, BOARD_LINE);
    }

    // 4. Vẽ quân cờ (tương đương PrintCell)
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

    // 5. Vẽ thông tin (tương đương DrawPlayerInfo)
    const char* turnText = g_turn ? "[X] Player 1" : "[O] Player 2";
    DrawTextEx(customFont,
        TextFormat("Lượt của: %s", turnText),
        Vector2{ (float)BOARD_OFFSET_X, (float)BOARD_OFFSET_Y - 40 }, 20.0f, 1.0f, WHITE); // (font, text, position, fontSize, spacing, color)

    DrawTextEx(customFont,
        "Nhấn [L] để Save",
        Vector2{ (float)BOARD_OFFSET_X, (float)BOARD_OFFSET_Y + BOARD_WIDTH + 15 }, 16.0f, 1.0f, WHITE);

    // 6. Vẽ thông báo thắng (tương đương ShowWinner)
    if (g_status != PLAYING) {
        // Vẽ một lớp phủ mờ
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), CLITERAL(Color){ 0, 0, 0, 150 });

        const char* winnerText = "";
        if (g_status == X_WIN) winnerText = "PLAYER 1 WINS!";
        else if (g_status == O_WIN) winnerText = "PLAYER 2 WINS!";
        else if (g_status == DRAW) winnerText = "IT'S A DRAW!";

        // Đo lường text để căn giữa
        Vector2 textSize = MeasureTextEx(customFont, winnerText, 40.0f, 1.0f);

        DrawTextEx(customFont,
            winnerText,
            Vector2{ (GetScreenWidth() - textSize.x) / 2, (float)(GetScreenHeight() / 2 - 40) },
            40.0f, 1.0f, GOLD);

        const char* restartText = "PRESS [Y] TO PLAY AGAIN";

        // Đo lường text để căn giữa
        textSize = MeasureTextEx(customFont, restartText, 20.0f, 1.0f);

        DrawTextEx(customFont,
            restartText,
            Vector2{ (GetScreenWidth() - textSize.x) / 2, (float)(GetScreenHeight() / 2 + 20) },
            20.0f, 1.0f, WHITE);
    }
}