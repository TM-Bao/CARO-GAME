#include "Game.h"
#include "raylib.h"
#include <fstream> //
#include <iostream> //

// Định nghĩa các biến toàn cục
int g_board[BOARD_SIZE][BOARD_SIZE];
bool g_turn;
GameStatus g_status;
int g_cursorX;
int g_cursorY;

// Tương đương resetData
void InitGame() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            g_board[i][j] = EMPTY;
        }
    }
    g_turn = true; // X đi trước
    g_status = PLAYING;
    //Reset con trỏ WASD
    g_cursorX = BOARD_SIZE / 2;
    g_cursorY = BOARD_SIZE / 2;

}

// Hàm này được gọi 60 lần/giây
void UpdateGame(GameScreen& currentScreen) {
    // Chỉ xử lý khi game đang diễn ra
    if (g_status != PLAYING) {
        // Nếu game kết thúc, chờ nhấn Y để chơi lại
        if (IsKeyPressed(KEY_Y)) { // Tương đương (toupper(_getch()) == 'Y')
            InitGame();
        }
        return;
    }
    if (IsKeyPressed(KEY_W) && g_cursorY > 0)
        g_cursorY--;

    if (IsKeyPressed(KEY_S) && g_cursorY < BOARD_SIZE - 1)
        g_cursorY++;

    if (IsKeyPressed(KEY_A) && g_cursorX > 0)
        g_cursorX--;

    if (IsKeyPressed(KEY_D) && g_cursorX < BOARD_SIZE - 1)
        g_cursorX++;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {

        if (CheckAndPlace(g_cursorX, g_cursorY)) {
            g_status = TestBoard();
            if (g_status == PLAYING)
                g_turn = !g_turn;
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        // Chuyển đổi tọa độ chuột sang tọa độ ô (i, j)
        int cellX = (mousePos.x - BOARD_OFFSET_X) / CELL_SIZE;
        int cellY = (mousePos.y - BOARD_OFFSET_Y) / CELL_SIZE;

        // Nếu click hợp lệ, gọi hàm logic
        if (cellX >= 0 && cellX < BOARD_SIZE && cellY >= 0 && cellY < BOARD_SIZE) {
            g_cursorX = cellX;
            g_cursorY = cellY;
            // "checkBoard" logic
            if (CheckAndPlace(cellX, cellY)) {
                // Nếu đặt cờ thành công, kiểm tra thắng
                g_status = TestBoard();

                if (g_status == PLAYING) {
                    g_turn = !g_turn; // Đổi lượt
                }
            }
        }
    }

    // Xử lý save/load
    if (IsKeyPressed(KEY_L)) { // Tương đương 'L'
        saveGame("save.txt");
    }
}

// "checkBoard" đã được "nâng cấp"
bool CheckAndPlace(int cellX, int cellY) {
    if (g_board[cellY][cellX] == EMPTY) {
        g_board[cellY][cellX] = (g_turn ? X : O);
        return true; // Đặt cờ thành công
    }
    return false; // Ô đã có người đánh
}

// "isFull" logic
bool isFull() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (g_board[i][j] == EMPTY) return false;
    return true;
}

// Logic testBoard giữ nguyên 100% từ file gốc
GameStatus TestBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Player c = (Player)g_board[i][j];
            if (c == EMPTY) continue;
            if (j + 4 < BOARD_SIZE && c == g_board[i][j + 1] && c == g_board[i][j + 2] && c == g_board[i][j + 3] && c == g_board[i][j + 4]) return (c == X) ? X_WIN : O_WIN;
            if (i + 4 < BOARD_SIZE && c == g_board[i + 1][j] && c == g_board[i + 2][j] && c == g_board[i + 3][j] && c == g_board[i + 4][j]) return (c == X) ? X_WIN : O_WIN;
            if (i + 4 < BOARD_SIZE && j + 4 < BOARD_SIZE && c == g_board[i + 1][j + 1] && c == g_board[i + 2][j + 2] && c == g_board[i + 3][j + 3] && c == g_board[i + 4][j + 4]) return (c == X) ? X_WIN : O_WIN;
            if (i + 4 < BOARD_SIZE && j >= 4 && c == g_board[i + 1][j - 1] && c == g_board[i + 2][j - 2] && c == g_board[i + 3][j - 3] && c == g_board[i + 4][j - 4]) return (c == X) ? X_WIN : O_WIN;
        }
    }
    return isFull() ? DRAW : PLAYING;
}

// Logic save/load giữ nguyên 100% từ file gốc
void saveGame(const std::string& filename) {
    std::ofstream f(filename);
    if (!f.is_open()) return;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) f << g_board[i][j] << " ";
        f << "\n";
    }
    f << g_turn;
    f.close();
}

void loadGame(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return; //
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int val; f >> val;
            g_board[i][j] = val;
        }
    }
    f >> g_turn;
    f.close();
    g_status = PLAYING; // Sẵn sàng chơi
}