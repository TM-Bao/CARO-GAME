#include "Game.h"
#include "raylib.h"
#include <fstream> //
#include <iostream> //

// Định nghĩa các biến toàn cục
int g_board[BOARD_SIZE][BOARD_SIZE];
bool g_turn;
int g_cursorX = 0;  // cột 0
int g_cursorY = 0;  // dòng 0
float g_moveTimer = 0.0f;
const float MOVE_DELAY = 0.15f; // 0.15 giây giữa 2 lần di chuyển
GameStatus g_status;

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
void UpdateGame() {
    // Chỉ xử lý khi game đang diễn ra
    if (g_status != PLAYING) {
        // Nếu game kết thúc, chờ nhấn Y để chơi lại
        if (IsKeyPressed(KEY_Y)) { // Tương đương (toupper(_getch()) == 'Y')
            InitGame();
        }
        return;
    }
    //Thời gina giữa các frame
    float dt = GetFrameTime(); 
    g_moveTimer += dt;

    //Di chuyển bằng W A S D
    if (g_moveTimer >= MOVE_DELAY) {
        bool moved = false;

        if (IsKeyDown(KEY_W) && g_cursorY > 0) {
            g_cursorY--;
            moved = true;
        }
        if (IsKeyDown(KEY_S) && g_cursorY < BOARD_SIZE - 1) {
            g_cursorY++;
            moved = true;
        }
        if (IsKeyDown(KEY_A) && g_cursorX > 0) {
            g_cursorX--;
            moved = true;
        }
        if (IsKeyDown(KEY_D) && g_cursorX < BOARD_SIZE - 1) {
            g_cursorX++;
            moved = true;
        }

        if (moved) g_moveTimer = 0.0f;
    }


    //Đánh cờ bằng ENTER
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (CheckAndPlace(g_cursorX, g_cursorY)) {
            g_status = TestBoard();  // kiểm tra thắng

            if (g_status == PLAYING)
                g_turn = !g_turn; // đổi lượt
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        int cellX = (int)floor((mousePos.x - BOARD_OFFSET_X) / (float)CELL_SIZE);
        int cellY = (int)floor((mousePos.y - BOARD_OFFSET_Y) / (float)CELL_SIZE);

        if (cellX >= 0 && cellX < BOARD_SIZE && cellY >= 0 && cellY < BOARD_SIZE) {
            // Đồng bộ con trỏ với vị trí click để highlight "ăn khớp"
            g_cursorX = cellX;
            g_cursorY = cellY;

            if (CheckAndPlace(cellX, cellY)) {
                g_status = TestBoard();
                if (g_status == PLAYING) g_turn = !g_turn;
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
    g_cursorX = 0;
    g_cursorY = 0;
}